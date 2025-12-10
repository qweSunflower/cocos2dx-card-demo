// view/StackView.h
#ifndef _STACK_VIEW_H_
#define _STACK_VIEW_H_
#pragma once

#include <functional>
#include <vector>
#include "2d/CCActionInterval.h"
#include "2d/CCNode.h"
#include "base/CCEvent.h"
#include "base/CCEventListenerMouse.h"
#include "math/Vec2.h"
#include "cocos2d.h"
#include "2d/CCLayer.h"
#include "models/GameModel.h"
#include "views/CardView.h"

class StackView : public cocos2d::Node {
    static cocos2d::Sequence* s_draw_card_animation;
    cocos2d::LayerColor* _backgroundLayer;
    // cocos2d::Node* _cardStack;
    // cocos2d::Node* _usedStack;
    std::vector<CardView*> _cardStack;
    std::vector<CardView*> _usedStack;
    cocos2d::EventListenerMouse* _mouseListener;
    void _onMouseDown(cocos2d::Event* event);
    std::function<void(int)> _onStackClickCallback;
public:
    static cocos2d::Vec2 s_used_stack_position;
    static const cocos2d::Vec2 S_STACK_POSITION;
    static const int S_INTERVAL;
    // static const cocos2d::Vec2& getStackPosition();
    static StackView* create();
    virtual bool init() override;
    void addCardView(CardView* cardView);
    CardView* popCardView();
    void addUsedCardView(CardView* cardView, bool reSetPositon=false);
    CardView* popUsedCardView();
    CardView* popCard();
    void moveCardToUsed();
    void moveUsedTOcard();
    void initCardPosition();
    bool playReMoveStackAnimation(bool isAdd);
    bool playDrawPlayFieldCardAnimation(CardView* cardView);
    bool playbackPlayFieldCardAnimation(CardView* cardView, cocos2d::Vec2& position);
    bool playDrawCardAnimation(CardView* cardView);
    bool playBackCardAnimation(CardView* cardView);
    void setOnStackClickCallback(std::function<void(int)> callback);
    cocos2d::Vec2 getUsedStackPosition();
};

#endif // !_STACK_VIEW_H_