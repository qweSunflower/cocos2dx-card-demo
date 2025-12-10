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

const cocos2d::Vec2 StackView::S_STACK_POSITION = cocos2d::Vec2(500, 580 / 2); //  定义卡牌堆叠的固定位置常量

cocos2d::Vec2 StackView::s_used_stack_position = cocos2d::Vec2(800, 580 / 2); //  定义已使用卡牌堆叠的位置变量

const int StackView::S_INTERVAL = 100; //  定义卡牌之间的间隔常量

cocos2d::Sequence* StackView::s_draw_card_animation = cocos2d::Sequence::create( //  创建抽牌动画序列，将卡牌移动到堆叠位置
    cocos2d::MoveTo::create(0.5f, StackView::S_STACK_POSITION), //  0.5秒内移动到堆叠位置
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
    _mouseListener = cocos2d::EventListenerMouse::create(); //  创建鼠标事件监听器
    _mouseListener->onMouseDown = CC_CALLBACK_1(StackView::_onMouseDown, this); //  绑定鼠标按下事件回调函数，使用CC_CALLBACK_1宏将StackView类的_onMouseDown方法绑定到当前实例
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this); //  将事件监听器以场景图优先级方式添加到事件分发器中
    return true;
}

void StackView::initCardPosition() {
    cocos2d::Vec2 positon = S_STACK_POSITION; //  获取堆牌的初始位置，并根据卡牌堆大小调整起始x坐标
    positon.x -= (_cardStack.size() - 1) * S_INTERVAL;
    for (auto cardView : _cardStack) { //  遍历卡牌堆，设置每张卡牌的位置，并向右偏移一定间隔
        cardView->setPosition(positon);
        positon.x += S_INTERVAL;
    }
    for (auto cardView : _usedStack) { //  遍历已使用卡牌堆，设置所有卡牌的位置为固定位置，并设置为不可见
        cardView->setPosition(StackView::s_used_stack_position);
        cardView->setVisible(false);
    }
    if (!_usedStack.empty()) /** * 如果使用的牌堆不为空，则显示最上面的一张牌 * 通过设置可见性为true来实现 */
        _usedStack.back()->setVisible(true);
}

void StackView::addCardView(CardView* cardView) {
    _cardStack.push_back(cardView); //  将卡牌视图添加到卡牌堆的末尾
    addChild(cardView); //  将卡牌视图作为子节点添加到当前节点
    cardView->release(); //  释放卡牌视图的引用计数
}

CardView* StackView::popCardView() {
    CardView* cardView = _cardStack.back(); //  获取栈顶的卡牌视图
    _cardStack.pop_back(); //  从栈中移除该卡牌
    cardView->retain(); //  增加引用计数，防止对象被释放
    cardView->removeFromParentAndCleanup(false); //  从父节点中移除卡牌，但不进行清理
    return cardView;
}

void StackView::addUsedCardView(CardView* cardView, bool reSetPositon) {
    _usedStack.push_back(cardView); //  将卡牌添加到已使用牌堆中
    if (reSetPositon) { //  如果需要重置位置
        cardView->setPosition(convertToNodeSpace((cardView->getWorldPosition()))); //  将卡牌的世界坐标转换为本节点坐标并设置位置
    }
    addChild(cardView); //  将卡牌作为子节点添加
    cardView->release(); //  减少引用计数，平衡之前的retain调用
}

CardView* StackView::popUsedCardView() {
    CardView* cardView = _usedStack.back(); //  获取并移除已使用牌堆的最后一个元素（即顶部卡牌）
    _usedStack.pop_back();
    cardView->setWorldPosition(convertToWorldSpace(cardView->getPosition())); //  设置卡牌的世界坐标位置，确保其位置正确
    cardView->retain(); //  增加卡牌的引用计数，防止被自动释放
    cardView->removeFromParent(); //  从父节点中移除卡牌
    if (_usedStack.size() > 0) { //  如果牌堆中还有卡牌，显示新的顶部卡牌
        _usedStack.back()->setVisible(true);
    }
    return cardView; //  返回被弹出的卡牌视图
}

void StackView::_onMouseDown(cocos2d::Event* event) {
    cocos2d::EventMouse* e = static_cast<cocos2d::EventMouse*>(event);
    auto pos = e->getLocationInView(); //  获取鼠标在视图坐标系中的位置
    if (_cardStack.empty() || e->getMouseButton() != cocos2d::EventMouse::MouseButton::BUTTON_LEFT) return; //  如果卡牌堆为空，或者按下的不是鼠标左键，则直接返回
    if (_cardStack.back()->containsPoint(pos) && _onStackClickCallback) { //  检查点击位置是否在卡牌堆最后一张卡牌的范围内，并且有注册的点击回调函数
        _onStackClickCallback(_cardStack.back()->getCardId()); //  如果条件满足，调用回调函数并传递最后一张卡牌的ID
    }
}

bool StackView::playDrawCardAnimation(CardView* cardView) {
    cardView->playCardFlipAnimation();
    cardView->runAction(cocos2d::Sequence::create(
        cocos2d::MoveTo::create(0.5f, StackView::s_used_stack_position),
        [&]() { /** * Lambda函数，用于处理栈中卡片的可见性 * 当栈中有多于一张卡片时，将倒数第二张卡片设为不可见 * [&] 表示捕获所有外部变量 by reference */
            if (_usedStack.size() > 1) { //  检查栈中卡片数量是否大于1
                _usedStack[_usedStack.size()-2]->setVisible(false); //  将倒数第二张卡片设为不可见
            }},
        nullptr
    ));
    return true;
}

bool StackView::playReMoveStackAnimation(bool isAdd) {
    int interval = S_INTERVAL;
    if (isAdd) interval *= -1; //  根据isAdd参数决定移动方向
    for (auto cardView : _cardStack) { //  遍历栈中的所有卡片，为每张卡片添加移动动作
        cardView->runAction(cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(interval, 0)));
    }
    return true;
}

bool StackView::playBackCardAnimation(CardView* cardView) {
    cardView->playCardFlipAnimation(); //  播放卡片翻转动画
    cardView->runAction(cocos2d::Sequence::create( //  创建一个序列动作
        cocos2d::MoveTo::create(0.5f, StackView::S_STACK_POSITION), //  移动到指定位置
        nullptr
    ));
    return true;
}


bool StackView::playDrawPlayFieldCardAnimation(CardView* cardView) {
    cardView->runAction(cocos2d::Sequence::create( //  创建并运行一个序列动画     1. 0.5秒内将卡牌移动到使用区域的位置     2. 动画完成后，如果使用区有多张卡牌，则隐藏倒数第二张卡牌
        cocos2d::MoveTo::create(0.5f, StackView::s_used_stack_position),
        [&]() {
            if (_usedStack.size() > 1) { //  检查使用区卡牌数量，如果大于1则隐藏倒数第二张卡牌
                _usedStack[_usedStack.size()-2]->setVisible(false);
            }},
        nullptr
    ));
    return true;
}
bool StackView::playbackPlayFieldCardAnimation(CardView* cardView, cocos2d::Vec2& position) {
    cardView->runAction(cocos2d::Sequence::create( //  创建并运行一个序列动作，使卡片视图在0.5秒内移动到指定位置     Sequence::create用于创建一系列连续执行的动作     MoveTo::create创建一个移动动作，第一个参数是持续时间，第二个参数是目标位置     nullptr作为序列的结束标记
        cocos2d::MoveTo::create(0.5f, position),
        nullptr
    ));
    return true;
}

void StackView::moveCardToUsed() {
    _usedStack.push_back(_cardStack.back()); //  将卡堆最后一张卡片添加到已使用卡堆
    _cardStack.pop_back(); //  从卡堆中移除最后一张卡片
}

void StackView::setOnStackClickCallback(std::function<void(int)> callback) {
    _onStackClickCallback = callback; //  设置卡堆点击回调函数
}