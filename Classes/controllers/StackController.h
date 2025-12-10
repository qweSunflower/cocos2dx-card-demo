// controllers/StackController.h
#ifndef _STACK_CONTROLLER_H_
#define _STACK_CONTROLLER_H_
#include "managers/UndoManager.h"
#include "views/CardView.h"
#pragma once

#include "models/GameModel.h"
#include "views/GameView.h"
#include "views/StackView.h"

class StackController {
    StackView* _stackView;
    UndoManager* _undoManage;
    std::function<void()> _cardMovedToUsedallback;
public:
    static StackController* create();
    StackController();
    void initView(GameModel* gameModel, GameView* gameView);
    void handelCardClick(int cardId);
    void addUsedCardView(CardView* cardView);
    CardView* popUsedCardView();
    void moveUsedToStack();
    void setCardMovedToUsedallback(std::function<void()> callback);
};

#endif // !_STACK_CONTROLLER_H_