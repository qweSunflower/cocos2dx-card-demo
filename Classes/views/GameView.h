// views/GameView.h
#ifndef _GAME_VIEW_H_
#define _GAME_VIEW_H_
#include "2d/CCLayer.h"
#include "2d/CCNode.h"
#include "ui/UIButton.h"
#pragma once

#include "2d/CCScene.h"
#include "PlayFieldView.h"
#include "ui/UIWidget.h"
#include <functional>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "views/PlayFieldView.h"
#include "views/StackView.h"

class GameView {
    cocos2d::Scene* _scene;
    PlayFieldView* _playFieldView;
    StackView* _stackView;
    cocos2d::ui::Button* _backButton;
    cocos2d::LayerColor* _fieldbackgroundLayer;
    cocos2d::LayerColor* _stackBackgroundLayer;
    void onMouseDown(cocos2d::Event *event);
    // void _onBackButtonClicked(cocos2d::Ref* pSender);
    cocos2d::ui::Widget::ccWidgetClickCallback _onBackButtonClickCallback;
    std::function<void(int)> _onCardClickCallback;
public:
    bool init();
    static GameView* create();
    void setOnBackButtonClickCallback(const cocos2d::ui::Widget::ccWidgetClickCallback& callback) {
        _onBackButtonClickCallback = callback;
    }
    void setPlayFieldView(PlayFieldView* playFieldView);
    void playMatchCardAnimatrion(CardView* cardView, cocos2d::Vec2 position, std::function<void(void)> callback);
    void setStackView(StackView* stackView);
    void setBackButtonVisible(bool visible);
};

#endif // !_GAME_VIEW_H_