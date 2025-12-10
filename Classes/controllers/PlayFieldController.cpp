// controllers/GameController.cpp
#include "controllers/PlayFieldController.h"
#include "base/ccMacros.h"
#include "views/CardView.h"
#include "views/GameView.h"
#include "views/PlayFieldView.h"
#include "services/CardMatchService.h"

PlayFieldController* PlayFieldController::create() {
    return new PlayFieldController();
}

void PlayFieldController::initView(GameModel* gameModel, GameView* gameView) {
    _gameModel = gameModel;
    _playFieldView = PlayFieldView::create();
    _playFieldView->setOnCardClickCallback(CC_CALLBACK_1(PlayFieldController::handleCardClick, this));
    _playFieldView->setPositionY(580);
    for (int cardIndex : gameModel->playfield) {
        CardModel* cardModel = gameModel->cardModels[cardIndex];
        CardView* cardView = CardView::create(cardModel);
        cardView->setCardId(cardIndex);
        cardView->retain();
        // cardView->setLocalZOrder(cardIndex);
        _playFieldView->addCardView(cardView);
    }
    gameView->setPlayFieldView(_playFieldView);
}

CardView* PlayFieldController::popCardView(int cardId) {
    return _playFieldView->popCardView(cardId);
}

void PlayFieldController::addCardView(CardView* cardView) {
    _playFieldView->addCardView(cardView, true);
}

void PlayFieldController::backUsedCard(CardView* cardView, cocos2d::Vec2& position) {
    _playFieldView->addCardView(cardView, true);
    _playFieldView->playBackCardAnimatrion(cardView, position);
}

void PlayFieldController::handleCardClick(int cardId) {
    if (CardMatchService::cardMatch(_gameModel, cardId)) {
        // _playFieldView->popCardView(cardId);
        // _playFieldView->playMatchCardAnimatrion();
        // _playFieldView->playMatchCardAnimatrion([&](){_cardMovedToStackCallback(cardId);});
        _cardMovedToStackCallback(cardId);
    }
}

void PlayFieldController::setCardMovedToStackCallback(std::function<void(int)> callback) {
    _cardMovedToStackCallback = callback;
}


void PlayFieldController::playCardFlipAnimation(int cardId) {
    _playFieldView->playCardFlipAnimation(cardId);
}