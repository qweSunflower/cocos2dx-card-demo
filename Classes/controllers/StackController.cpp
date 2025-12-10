
#include "StackController.h"
#include "base/ccMacros.h"
#include "views/CardView.h"

StackController* StackController::create() {
    return new StackController();
}

StackController::StackController() {}

void StackController::initView(GameModel* gameModel, GameView* gameView) {
    _stackView = StackView::create(); //  创建堆栈视图
    for (int cardIndex : gameModel->backupDesk) { //  遍历备份桌面上的卡片索引
        CardModel* cardModel = gameModel->cardModels[cardIndex];
        CardView* cardView = CardView::create(cardModel); //  创建卡片视图
        cardView->setCardId(cardIndex); //  设置卡片ID
        cardView->retain(); //  引用计数加1，防止对象被释放
        _stackView->addCardView(cardView); //  将卡片视图添加到堆栈视图中
    }
    for (int cardIndex : gameModel->cardDesk) { //  遍历桌面上的卡片索引
        CardModel* cardModel = gameModel->cardModels[cardIndex]; //  获取卡片模型
        CardView* cardView = CardView::create(cardModel); //  创建卡片视图
        cardView->setCardId(cardIndex); //  设置卡片ID
        cardView->retain(); //  引用计数加1，防止对象被释放
        _stackView->addUsedCardView(cardView); //  将卡片视图添加到已使用卡片视图中
    }
    _stackView->initCardPosition(); //  初始化卡片位置
    _stackView->setOnStackClickCallback(CC_CALLBACK_1(StackController::handelCardClick, this)); //  设置卡片点击回调函数
    gameView->setStackView(_stackView); //  将堆栈视图设置到游戏视图中
}

void StackController::handelCardClick(int cardId) {
    if (_cardMovedToUsedallback) _cardMovedToUsedallback(); //  如果有卡片移动到使用区的回调函数，则执行该回调
    CardView* cardView = _stackView->popCardView(); //  从牌堆视图中弹出一张卡片视图
    _stackView->addUsedCardView(cardView); //  将该卡片视图添加到使用区
    _stackView->playDrawCardAnimation(cardView); //  播放抽卡动画
    _stackView->playReMoveStackAnimation(false); //  播放移除牌堆动画，参数false表示不是从使用区移回牌堆
}

void StackController::addUsedCardView(CardView* cardView) {
    _stackView->addUsedCardView(cardView, true); //  向使用区添加卡片视图，并设置是否需要动画
    _stackView->playDrawPlayFieldCardAnimation(cardView); //  播放卡片进入游戏区域的动画
}

CardView* StackController::popUsedCardView() {
    return _stackView->popUsedCardView();
}

void StackController::setCardMovedToUsedallback(std::function<void(void)> callback) {
    _cardMovedToUsedallback = callback;
}

void StackController::moveUsedToStack() {
    _stackView->playReMoveStackAnimation(true); //  播放移回牌堆的动画，参数true表示是从使用区移回牌堆
    auto cardView = _stackView->popUsedCardView(); //  从使用区弹出一张卡片视图
    _stackView->addCardView(cardView); //  将卡片视图添加回牌堆
    _stackView->playBackCardAnimation(cardView); //  播放卡片返回牌堆的动画
}