
#include "StackController.h"
#include "base/ccMacros.h"
#include "views/CardView.h"

StackController* StackController::create() {
    return new StackController();
}

StackController::StackController() {}

void StackController::initView(GameModel* gameModel, GameView* gameView) {
    _stackView = StackView::create();
    for (int cardIndex : gameModel->backupDesk) {
        CardModel* cardModel = gameModel->cardModels[cardIndex];
        CardView* cardView = CardView::create(cardModel);
        cardView->setCardId(cardIndex);
        cardView->retain();
        _stackView->addCardView(cardView);
    }
    for (int cardIndex : gameModel->cardDesk) {
        CardModel* cardModel = gameModel->cardModels[cardIndex];
        CardView* cardView = CardView::create(cardModel);
        cardView->setCardId(cardIndex);
        cardView->retain();
        _stackView->addUsedCardView(cardView);
    }
    _stackView->initCardPosition();
    _stackView->setOnStackClickCallback(CC_CALLBACK_1(StackController::handelCardClick, this));
    gameView->setStackView(_stackView);
}

void StackController::handelCardClick(int cardId) {
    if (_cardMovedToUsedallback) _cardMovedToUsedallback();
    CardView* cardView = _stackView->popCardView();
    _stackView->addUsedCardView(cardView);
    _stackView->playDrawCardAnimation(cardView);
    _stackView->playReMoveStackAnimation(false);
}

void StackController::addUsedCardView(CardView* cardView) {
    _stackView->addUsedCardView(cardView, true);
    _stackView->playDrawPlayFieldCardAnimation(cardView);
}

CardView* StackController::popUsedCardView() {
    return _stackView->popUsedCardView();
}

void StackController::setCardMovedToUsedallback(std::function<void(void)> callback) {
    _cardMovedToUsedallback = callback;
}

void StackController::moveUsedToStack() {
    _stackView->playReMoveStackAnimation(true);
    auto cardView = _stackView->popUsedCardView();
    _stackView->addCardView(cardView);
    _stackView->playBackCardAnimation(cardView);
}