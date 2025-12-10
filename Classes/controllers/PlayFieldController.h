// controller/PlayFieldController.h
#ifndef _PLAY_FIELD_CONTROLLER_H_
#define _PLAY_FIELD_CONTROLLER_H_
#include "math/Vec2.h"
#include "views/CardView.h"
#include <functional>
#pragma once

#include "views/PlayFieldView.h"
#include "models/GameModel.h"
#include "views/GameView.h"

class PlayFieldController{
    GameModel* _gameModel;
    PlayFieldView* _playFieldView;
    std::function<void(int)> _cardMovedToStackCallback;
public:
    static PlayFieldController* create();
    void initView(GameModel* gameModel, GameView* gameView);
    void handleCardClick(int cardId);
    CardView* popCardView(int cardId);
    void addCardView(CardView* cardView);
    void backUsedCard(CardView* cardView, cocos2d::Vec2& position);
    void setCardMovedToStackCallback(std::function<void(int)> callback);
    void playCardFlipAnimation(int cardId);
};

#endif // !_PLAY_FIELD_CONTROLLER_H_