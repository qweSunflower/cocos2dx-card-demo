// controllers/GameController.cpp
#include "GameController.h"
#include "2d/CCActionInterval.h"
#include "base/ccMacros.h"
#include "configs/loader/LevelConfigLoader.h"
#include "math/Vec2.h"
#include "models/GameModel.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"
#include "services/GameModelFromLevelGenerator.h"
#include "managers/UndoManager.h"
#include "views/CardView.h"
#include "views/GameView.h"
#include <algorithm>

GameController* GameController::create() {
	return new GameController();
}

void GameController::startGame(int levelId) {
	auto levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
	_gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
	_playFieldController = PlayFieldController::create();
	_stackController = StackController::create();
	_undoManager = UndoManager::create();
	initView(_gameModel);
	_stackController->initView(_gameModel, _gameView);
	_stackController->setCardMovedToUsedallback(CC_CALLBACK_0(GameController::handleStackMoveToUsed, this));
};

void GameController::initView(GameModel* gameModel) {
	_gameView = GameView::create();
	_gameView->setOnBackButtonClickCallback(CC_CALLBACK_1(GameController::handleBackButtonClick, this));
	_gameView->setBackButtonVisible(false);
}

void GameController::handleBackButtonClick(cocos2d::Ref* sender) {
	if (_undoManager->empty()) return;
	auto record = _undoManager->popRecord();
	_gameView->setBackButtonVisible(!_undoManager->empty());
	if (record.isPlayField) {
		_gameModel->playfield.push_back(_gameModel->cardDesk.back());
		_gameModel->cardDesk.pop_back();
		auto cardView = _stackController->popUsedCardView();
		_playFieldController->backUsedCard(cardView, record.oldPosition);
		for (int id : _gameModel->cardModels[cardView->getCardId()]->downCards) {
			auto& cardModel = _gameModel->cardModels[id];
			if (cardModel->upCardNum == 0) {
				_playFieldController->playCardFlipAnimation(id);
				cardModel->flip = true;
			}
			_gameModel->cardModels[id]->upCardNum ++;
		}
	} else {
		_gameModel->backupDesk.push_back(_gameModel->cardDesk.back());
		_gameModel->cardDesk.pop_back();
		_stackController->moveUsedToStack();
	}
}

void GameController::handlePlayFieldMoveToStack(int cardId) {
	_gameModel->cardDesk.push_back(cardId);
	for (auto it = _gameModel->playfield.begin(); it < _gameModel->playfield.end(); it++) {
		if (*it == cardId) {
			_gameModel->playfield.erase(it);
			break;
		}
	}
	for (int id : _gameModel->cardModels[cardId]->downCards) {
		auto& cardModel = _gameModel->cardModels[id];
		cardModel->upCardNum --;
		if (cardModel->upCardNum == 0) {
			_playFieldController->playCardFlipAnimation(id);
			cardModel->flip = false;
		}
	}
	CardView* cardView = _playFieldController->popCardView(cardId);
	_undoManager->addRecord(true, cardView->getPosition());
	_gameView->setBackButtonVisible(!_undoManager->empty());
	_stackController->addUsedCardView(cardView);
}

void GameController::handleStackMoveToUsed() {
	_gameModel->cardDesk.push_back(_gameModel->backupDesk.back());
	_gameModel->backupDesk.pop_back();
	_undoManager->addRecord(false, cocos2d::Vec2(0,0));
	_gameView->setBackButtonVisible(!_undoManager->empty());
}