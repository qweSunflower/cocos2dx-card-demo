// controllers/GameController.cpp
#include "GameController.h"
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

GameController* GameController::create() {
	return new GameController();
}

void GameController::startGame(int levelId) {
	auto levelConfig = LevelConfigLoader::loadLevelConfig(levelId); //  加载指定ID的关卡配置
	_gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig); //  根据关卡配置生成游戏模型
	_playFieldController = PlayFieldController::create(); //  创建游戏区域控制器
	_stackController = StackController::create(); //  创建牌堆控制器
	_undoManager = UndoManager::create(); //  创建撤销管理器
	initView(_gameModel); //  初始化游戏视图
	_stackController->initView(_gameModel, _gameView); //  初始化牌堆视图并设置回调函数
	_stackController->setCardMovedToUsedallback(CC_CALLBACK_0(GameController::handleStackMoveToUsed, this));
	_playFieldController->initView(_gameModel, _gameView);
	_playFieldController->setCardMovedToStackCallback(CC_CALLBACK_1(GameController::handlePlayFieldMoveToStack, this));
};

void GameController::initView(GameModel* gameModel) {
	_gameView = GameView::create(); //  创建游戏视图实例
	_gameView->setOnBackButtonClickCallback(CC_CALLBACK_1(GameController::handleBackButtonClick, this)); //  设置返回按钮点击回调函数
	_gameView->setBackButtonVisible(false); //  初始状态下隐藏返回按钮
}

void GameController::handleBackButtonClick(cocos2d::Ref* sender) {
	if (_undoManager->empty()) return; //  如果撤销管理器为空，则直接返回
	auto record = _undoManager->popRecord(); //  弹出最近的操作记录
	_gameView->setBackButtonVisible(!_undoManager->empty()); //  根据撤销管理器是否为空来设置返回按钮的可见性
	if (record.isPlayField) { //  判断操作记录是否涉及游戏场地
		_gameModel->playfield.push_back(_gameModel->cardDesk.back());
		_gameModel->cardDesk.pop_back();
		auto cardView = _stackController->popUsedCardView(); //  从已使用卡片堆中弹出卡片视图
		_playFieldController->backUsedCard(cardView, record.oldPosition); //  将卡片视图放回原来的位置
		for (int id : _gameModel->cardModels[cardView->getCardId()]->downCards) {
			auto& cardModel = _gameModel->cardModels[id];
			if (cardModel->upCardNum == 0) { //  如果卡片没有压在上面的卡片
				_playFieldController->playCardFlipAnimation(id); //  播放卡片翻转动画
				cardModel->flip = true; //  设置卡片为翻转状态
			}
			_gameModel->cardModels[id]->upCardNum ++;
		}
	} else {
		_gameModel->backupDesk.push_back(_gameModel->cardDesk.back());
		_gameModel->cardDesk.pop_back(); //  从使用卡牌中移除最后一张卡牌
		_stackController->moveUsedToStack(); //  将已使用卡牌堆中的卡牌移回牌堆
	}
}

void GameController::handlePlayFieldMoveToStack(int cardId) {
	_gameModel->cardDesk.push_back(cardId); //  将指定卡牌添加到桌面卡牌中
	for (auto it = _gameModel->playfield.begin(); it < _gameModel->playfield.end(); it++) {
		if (*it == cardId) { //  找到匹配的卡牌ID
			_gameModel->playfield.erase(it); //  从游戏区域移除该卡牌
			break;
		}
	}
	for (int id : _gameModel->cardModels[cardId]->downCards) {
		auto& cardModel = _gameModel->cardModels[id];
		cardModel->upCardNum --; //  减少卡牌模型的upCardNum值
		if (cardModel->upCardNum == 0) { //  如果upCardNum减少到0，则执行卡牌翻转动画
			_playFieldController->playCardFlipAnimation(id); //  播放卡牌翻转动画
			cardModel->flip = false; //  将卡牌的flip状态设为false
		}
	}
	CardView* cardView = _playFieldController->popCardView(cardId);
	_undoManager->addRecord(true, cardView->getPosition()); //  添加撤销记录，记录卡牌位置 第一个参数true表示这是一个卡牌移动操作 第二个参数是卡牌的位置
	_gameView->setBackButtonVisible(!_undoManager->empty()); //  根据撤销管理器是否为空来设置返回按钮的可见性
	_stackController->addUsedCardView(cardView); //  将卡牌视图添加到已使用卡牌堆控制器中
}

void GameController::handleStackMoveToUsed() {
	_gameModel->cardDesk.push_back(_gameModel->backupDesk.back()); //  将备份区的最后一张卡牌添加到卡牌桌中
	_gameModel->backupDesk.pop_back(); //  从备份区移除最后一张卡牌
	_undoManager->addRecord(false, cocos2d::Vec2(0,0)); //  添加撤销记录     第一个参数false表示这不是卡牌移动操作     第二个参数(0,0)表示没有位置信息需要记录
	_gameView->setBackButtonVisible(!_undoManager->empty()); //  根据撤销管理器是否为空来设置返回按钮的可见性
}