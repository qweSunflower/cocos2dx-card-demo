// views/StackView.cpp
#include "views/StackView.h"
#include "2d/CCActionInterval.h"
#include "2d/CCLayer.h"
#include "2d/CCNode.h"
#include "CardView.h"
#include "base/CCEvent.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerMouse.h"
#include "base/CCEventMouse.h"
#include "base/ccMacros.h"
#include "math/Vec2.h"
#include <cstddef>

const cocos2d::Vec2 StackView::S_STACK_POSITION = cocos2d::Vec2(500, 580 / 2);

cocos2d::Vec2 StackView::s_used_stack_position = cocos2d::Vec2(800, 580 / 2);

const int StackView::S_INTERVAL = 100;

cocos2d::Sequence* StackView::s_draw_card_animation = cocos2d::Sequence::create(
    cocos2d::MoveTo::create(0.5f, StackView::S_STACK_POSITION),
    nullptr
);

StackView* StackView::create() {
    StackView* ret = new (std::nothrow) StackView();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool StackView::init() {
    // _backgroundLayer = cocos2d::LayerColor::create(cocos2d::Color4B(128, 0, 0, 255), 1080, 580);
    // _backgroundLayer->setLocalZOrder(-10);
    // addChild(_backgroundLayer);
    _mouseListener = cocos2d::EventListenerMouse::create();
    _mouseListener->onMouseDown = CC_CALLBACK_1(StackView::_onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
    return true;
}

void StackView::initCardPosition() {
    cocos2d::Vec2 positon = S_STACK_POSITION;
    positon.x -= (_cardStack.size() - 1) * S_INTERVAL;
    for (auto cardView : _cardStack) {
        cardView->setPosition(positon);
        positon.x += S_INTERVAL;
    }
    for (auto cardView : _usedStack) {
        cardView->setPosition(StackView::s_used_stack_position);
        cardView->setVisible(false);
    }
    if (!_usedStack.empty())
        _usedStack.back()->setVisible(true);
}

void StackView::addCardView(CardView* cardView) {
    _cardStack.push_back(cardView);
    addChild(cardView);
    cardView->release();
}

CardView* StackView::popCardView() {
    CardView* cardView = _cardStack.back();
    _cardStack.pop_back();
    cardView->retain();
    // removeChild(cardView, false);
    cardView->removeFromParentAndCleanup(false);
    return cardView;
}

void StackView::addUsedCardView(CardView* cardView, bool reSetPositon) {
    _usedStack.push_back(cardView);
    if (reSetPositon) {
        cardView->setPosition(convertToNodeSpace((cardView->getWorldPosition())));
    }
    // cardView->setPosition(s_used_stack_position);
    addChild(cardView);
    cardView->release();
    // cardView->setParent(this);
}

CardView* StackView::popUsedCardView() {
    CardView* cardView = _usedStack.back();
    _usedStack.pop_back();
    // removeChild(cardView, false);
    cardView->setWorldPosition(convertToWorldSpace(cardView->getPosition()));
    cardView->retain();
    cardView->removeFromParent();
    if (_usedStack.size() > 0) {
        _usedStack.back()->setVisible(true);
    }
    return cardView;
}

void StackView::_onMouseDown(cocos2d::Event* event) {
    cocos2d::EventMouse* e = static_cast<cocos2d::EventMouse*>(event);
    auto pos = e->getLocationInView();
    if (_cardStack.empty() || e->getMouseButton() != cocos2d::EventMouse::MouseButton::BUTTON_LEFT) return;
    if (_cardStack.back()->containsPoint(pos) && _onStackClickCallback) {
        _onStackClickCallback(_cardStack.back()->getCardId());
    }
}

bool StackView::playDrawCardAnimation(CardView* cardView) {
    cardView->playCardFlipAnimation();
    cardView->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.5f, StackView::s_used_stack_position),
        [&]() {
            if (_usedStack.size() > 1) {
                _usedStack[_usedStack.size()-2]->setVisible(false);
            }},
        nullptr
    ));
    // for (auto cardView : _cardStack) {
    //     cardView->runAction(cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(S_INTERVAL, 0)));
    // }
    return true;
}

bool StackView::playReMoveStackAnimation(bool isAdd) {
    int interval = S_INTERVAL;
    if (isAdd) interval *= -1;
    for (auto cardView : _cardStack) {
        cardView->runAction(cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(interval, 0)));
    }
    return true;
}

bool StackView::playBackCardAnimation(CardView* cardView) {
    // for (auto cardView : _cardStack) {
    //     cardView->runAction(cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(-S_INTERVAL, 0)));
    // }
    cardView->playCardFlipAnimation();
    cardView->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.5f, StackView::S_STACK_POSITION),
        nullptr
    ));
    return true;
}


bool StackView::playDrawPlayFieldCardAnimation(CardView* cardView) {
    cardView->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.5f, StackView::s_used_stack_position),
        [&]() {
            if (_usedStack.size() > 1) {
                _usedStack[_usedStack.size()-2]->setVisible(false);
            }},
        nullptr
    ));
    return true;
}
bool StackView::playbackPlayFieldCardAnimation(CardView* cardView, cocos2d::Vec2& position) {
    cardView->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.5f, position),
        nullptr
    ));
    return true;
}

void StackView::moveCardToUsed() {
    _usedStack.push_back(_cardStack.back());
    _cardStack.pop_back();
}

void StackView::setOnStackClickCallback(std::function<void(int)> callback) {
    _onStackClickCallback = callback;
}