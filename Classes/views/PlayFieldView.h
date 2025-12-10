// view/PlayFieldView.h
#ifndef _PLAY_FIELD_VIEW_H_
#define _PLAY_FIELD_VIEW_H_
#include "math/Vec2.h"
#include "views/StackView.h"
#include <functional>
#pragma once

#include "2d/CCLayer.h"
#include "2d/CCNode.h"
#include "base/CCEventListenerMouse.h"
#include <vector>
#include "views/CardView.h"
#include "cocos2d.h"

class PlayFieldView : public cocos2d::Node  {
    cocos2d::LayerColor* _backgroundLayer;
    std::vector<CardView*> _cardViews;
    cocos2d::EventListenerMouse* _mouseListener;
    void _onMouseDown(cocos2d::Event* event);
    std::function<void(int)> _onCardClickCallback;
    CardView* _clickCardView;
public:
    static PlayFieldView* create();
    virtual bool init() override;
    void moveCardToUsed(int cardId);
    void addCardView(CardView* cardView, bool reSetPositon=false);
    CardView* popCardView(int cardId);
    std::vector<CardView*>::iterator getCardViewIt(int cardId);
    void playMatchCardAnimatrion(std::function<void(void)> callback);
    void playBackCardAnimatrion(CardView* cardView, cocos2d::Vec2& position);
    void setOnCardClickCallback(std::function<void(int)> callback);
    void playCardFlipAnimation(int cardId);
};

#endif // !_PLAY_FIELD_VIEW_H_