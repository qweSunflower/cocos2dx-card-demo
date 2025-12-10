// views/PlayFieldView.cpp
#include "PlayFieldView.h"
#include "CardView.h"
#include "math/Vec2.h"
#include "2d/CCActionInterval.h"
#include "views/StackView.h"

PlayFieldView* PlayFieldView::create() {
    PlayFieldView* ret = new (std::nothrow) PlayFieldView();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool PlayFieldView::init() {
    // _backgroundLayer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 128, 0, 255), 1080, 1500);
    // _backgroundLayer->setLocalZOrder(-10);
    // addChild(_backgroundLayer);
    _mouseListener = cocos2d::EventListenerMouse::create();
    _mouseListener->onMouseDown = CC_CALLBACK_1(PlayFieldView::_onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
    // _backgroundLayer->setPositionY(580);
    return true;
}

void PlayFieldView::addCardView(CardView* cardView, bool reSetPositon) {
    _cardViews.push_back(cardView);
    if (reSetPositon) {
        cardView->setPosition(convertToNodeSpace(cardView->getWorldPosition()));
    }
    addChild(cardView);
    cardView->release();
}


std::vector<CardView*>::iterator PlayFieldView::getCardViewIt(int cardId) {
    auto it = _cardViews.begin();
    while (it < _cardViews.end() && (*it)->getCardId() != cardId) {
        it ++;
    }
    if (it == _cardViews.end())
        return _cardViews.end();
    return it;
}

CardView* PlayFieldView::popCardView(int cardId) {
    auto it = getCardViewIt(cardId);
    CardView* cardView = *it;
    _cardViews.erase(it);
    cardView->setWorldPosition(convertToWorldSpace(cardView->getPosition()));
    cardView->retain();
    cardView->removeFromParent();
    return cardView;
}


void PlayFieldView::_onMouseDown(cocos2d::Event* event) {
    cocos2d::EventMouse* e = static_cast<cocos2d::EventMouse*>(event);
    auto pos = e->getLocationInView();
    if (_cardViews.empty() || e->getMouseButton() != cocos2d::EventMouse::MouseButton::BUTTON_LEFT) return;
    for (auto cardIt = _cardViews.rbegin(); cardIt < _cardViews.rend(); cardIt++) {
        if ((*cardIt)->containsPoint(pos) && _onCardClickCallback) {
            _clickCardView = (*cardIt);
            _onCardClickCallback((*cardIt)->getCardId());
            break;
        }
    }
}

void PlayFieldView::setOnCardClickCallback(std::function<void(int)> callback) {
    _onCardClickCallback = callback;
}

void PlayFieldView::playBackCardAnimatrion(CardView* cardView, cocos2d::Vec2& position) {
    cardView->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.5f, position),
        nullptr
    ));
}

void PlayFieldView::playMatchCardAnimatrion(std::function<void(void)> callback) {
    _clickCardView->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.5f, StackView::s_used_stack_position),
        cocos2d::CallFunc::create(callback),
        nullptr
    ));
}


void PlayFieldView::playCardFlipAnimation(int cardId) {
    auto cardView = *getCardViewIt(cardId);
    cardView->playCardFlipAnimation();
}