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
    _mouseListener = cocos2d::EventListenerMouse::create(); //  创建鼠标事件监听器
    _mouseListener->onMouseDown = CC_CALLBACK_1(PlayFieldView::_onMouseDown, this); //  设置鼠标按下事件的回调函数，绑定当前类的_onMouseDown方法
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this); //  以场景图优先级将监听器注册到事件分发器
    return true;
}

void PlayFieldView::addCardView(CardView* cardView, bool reSetPositon) {
    _cardViews.push_back(cardView); //  将卡牌视图添加到容器中
    if (reSetPositon) { //  如果需要重置位置
        cardView->setPosition(convertToNodeSpace(cardView->getWorldPosition())); //  将世界坐标转换为节点本地坐标并设置位置
    }
    addChild(cardView); //  将卡牌视图作为子节点添加
    cardView->release(); //  释放卡牌视图的引用计数
}


std::vector<CardView*>::iterator PlayFieldView::getCardViewIt(int cardId) {
    auto it = _cardViews.begin();
    while (it < _cardViews.end() && (*it)->getCardId() != cardId) {
        it ++;
    }
    if (it == _cardViews.end()) //  如果遍历完整个容器仍未找到匹配项，返回容器末尾迭代器
        return _cardViews.end();
    return it; //  返回找到的匹配项的迭代器
}

CardView* PlayFieldView::popCardView(int cardId) {
    auto it = getCardViewIt(cardId); //  获取指定ID的卡片视图迭代器
    CardView* cardView = *it; //  获取迭代器指向的卡片视图
    _cardViews.erase(it); //  从容器中移除该卡片视图
    cardView->setWorldPosition(convertToWorldSpace(cardView->getPosition())); //  将卡片视图的位置转换为世界坐标
    cardView->retain(); //  增加引用计数，防止对象被释放
    cardView->removeFromParent(); //  从父节点中移除卡片视图
    return cardView; //  返回弹出的卡片视图
}


void PlayFieldView::_onMouseDown(cocos2d::Event* event) {
    cocos2d::EventMouse* e = static_cast<cocos2d::EventMouse*>(event); //  将事件对象转换为鼠标事件对象，以获取鼠标相关的具体信息
    auto pos = e->getLocationInView(); //  获取鼠标在视图中的点击位置坐标
    if (_cardViews.empty() || e->getMouseButton() != cocos2d::EventMouse::MouseButton::BUTTON_LEFT) return; //  如果没有卡牌视图或者点击的不是鼠标左键，则直接返回
    for (auto cardIt = _cardViews.rbegin(); cardIt < _cardViews.rend(); cardIt++) { //  从后向前遍历卡牌视图集合，这样最上层的卡牌会被优先检测
        if ((*cardIt)->containsPoint(pos) && _onCardClickCallback) { //  检查点击位置是否在当前卡牌范围内，并且存在卡牌点击回调函数
            _clickCardView = (*cardIt); //  记录被点击的卡牌视图
            _onCardClickCallback((*cardIt)->getCardId()); //  调用卡牌点击回调函数，并传入被点击卡牌的ID
            break; //  找到被点击的卡牌后，退出循环
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