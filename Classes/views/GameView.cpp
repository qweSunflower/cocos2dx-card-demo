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
    _scene = Scene::create(); //  创建场景并设置背景层
    _fieldbackgroundLayer = cocos2d::LayerColor::create(cocos2d::Color4B(180, 180, 0, 255), 1080, 1500); //  创建游戏区域背景层，设置颜色为黄色(180,180,0)和透明度255，尺寸为1080x1500
    _fieldbackgroundLayer->setPositionY(580); //  设置Y轴位置
    _stackBackgroundLayer = cocos2d::LayerColor::create(cocos2d::Color4B(200, 0, 80, 255), 1080, 580); //  创建堆叠区域背景层，设置颜色为深红色(200,0,80)和透明度255，尺寸为1080x580
    _backButton = ui::Button::create("res/ui_images/backbutton.png"); //  创建返回按钮
    _backButton->setScale(150/_backButton->getContentSize().width, 150/_backButton->getContentSize().height); //  设置按钮缩放比例，使按钮大小为150x150
    _backButton->addClickEventListener([&](Ref* sender) { //  添加按钮点击事件监听器
        if (_onBackButtonClickCallback) {
            _onBackButtonClickCallback(sender);
        }
    });
    _backButton->setPosition(Vec2(1080-100, 290)); //  设置返回按钮位置在右上角(1080-100, 290)
    _scene->addChild(_fieldbackgroundLayer); //  将背景层添加到场景中
    _scene->addChild(_stackBackgroundLayer);
    _scene->addChild(_backButton, 1);
    Director::getInstance()->runWithScene(_scene); /** * 运行场景并返回成功状态 * 使用Director单例运行传入的场景 * @return 返回true表示场景运行成功 */
    return true;
}

void GameView::setPlayFieldView(PlayFieldView* playFieldView) {
    _playFieldView = playFieldView; //  保存游戏场地视图指针
    _scene->addChild(playFieldView); //  将游戏场地视图添加到场景中
}

void GameView::setStackView(StackView* stackView) {
    _stackView = stackView; //  保存牌堆视图指针
    _scene->addChild(stackView); //  将牌堆视图添加到场景中
}


void GameView::setBackButtonVisible(bool visible) {
    _backButton->setVisible(visible); //  设置返回按钮的可见性
}

void GameView::playMatchCardAnimatrion(CardView* cardView, cocos2d::Vec2 position, std::function<void(void)> callback) {
    _scene->addChild(cardView); //  将卡片视图添加到场景中
    cardView->runAction(Sequence::create( //  创建并执行一个序列动作，包含以下步骤：
        MoveTo::create(0.5f, position), //  1. 在0.5秒内移动到指定位置
        CallFunc::create([&](){cardView->removeFromParent();}), //  2. 从父节点中移除卡片视图
        CallFunc::create(callback), //  3. 执行回调函数
        nullptr
    ));
}