#include "GameView.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"
#include "2d/CCLayer.h"
#include "PlayFieldView.h"
#include "base/CCDirector.h"
#include "base/ccMacros.h"
#include "math/Vec2.h"
#include "platform/CCPlatformMacros.h"
#include "views/StackView.h"
#include <cstddef>

USING_NS_CC;

GameView* GameView::create() {
    auto ret = new GameView();
    ret->init();
    return ret;
}

bool GameView::init() {
    _scene = Scene::create();
    _fieldbackgroundLayer = cocos2d::LayerColor::create(cocos2d::Color4B(180, 180, 0, 255), 1080, 1500);
    _fieldbackgroundLayer->setPositionY(580);
    _stackBackgroundLayer = cocos2d::LayerColor::create(cocos2d::Color4B(200, 0, 80, 255), 1080, 580);
    _backButton = ui::Button::create("res/ui_images/backbutton.png");
    _backButton->setScale(150/_backButton->getContentSize().width, 150/_backButton->getContentSize().height);
    _backButton->addClickEventListener([&](Ref* sender) {
        if (_onBackButtonClickCallback) {
            _onBackButtonClickCallback(sender);
        }
    });
    _backButton->setPosition(Vec2(1080-100, 290));
    _scene->addChild(_fieldbackgroundLayer);
    _scene->addChild(_stackBackgroundLayer);
    _scene->addChild(_backButton, 1);
    Director::getInstance()->runWithScene(_scene);
    return true;
}

void GameView::setPlayFieldView(PlayFieldView* playFieldView) {
    _playFieldView = playFieldView;
    _scene->addChild(playFieldView);
}

void GameView::setStackView(StackView* stackView) {
    _stackView = stackView;
    _scene->addChild(stackView);
}


void GameView::setBackButtonVisible(bool visible) {
    _backButton->setVisible(visible);
}

void GameView::playMatchCardAnimatrion(CardView* cardView, cocos2d::Vec2 position, std::function<void(void)> callback) {
    _scene->addChild(cardView);
    cardView->runAction(Sequence::create(
        MoveTo::create(0.5f, position),
        CallFunc::create([&](){cardView->removeFromParent();}),
        CallFunc::create(callback),
        nullptr
    ));
}