#include "CardView.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"
#include "configs/models/CardResConfig.h"
#include "math/Vec2.h"
#include "models/CardModel.h"

CardView* CardView::create(CardModel* cardModel) {
    CardView* ret = new (std::nothrow) CardView(); //  使用no-throw方式创建CardView对象
    if (ret && ret->initWithCardModel(cardModel)) { //  检查对象创建及初始化是否成功
        ret->autorelease(); //  如果成功，设置对象为自动释放
        return ret; //  返回创建的对象
    }
    delete ret; //  如果失败，删除对象
    return nullptr; //  返回空指针
}

bool CardView::initWithCardModel(CardModel* cardModel) {
    setPosition(cardModel->postion); //  设置卡片位置为模型中的位置
    _faceGeneralSprite = cocos2d::Sprite::create(CardResConfig::faceGeneralPath); //  创建卡片正面精灵
    _backGeneralSprite = cocos2d::Sprite::create(CardResConfig::backGeneralPath); //  创建卡片背面精灵
    addChild(_faceGeneralSprite); //  将正面和背面的精灵添加到父节点中
    addChild(_backGeneralSprite);
    const auto& cardResConfig = CardResConfig::getCardResConfig(cardModel->suit, cardModel->face);
    _numberSprite = cocos2d::Sprite::create(cardResConfig->numberPath); //  创建数字精灵，使用卡牌资源配置中的数字路径
    _smallNumberSprite = cocos2d::Sprite::create(cardResConfig->smallNumberPath); //  创建小数字精灵，使用卡牌资源配置中的小数字路径
    _suitSprite = cocos2d::Sprite::create(cardResConfig->suitPath); //  创建花色精灵，使用卡牌资源配置中的花色路径
    _numberSprite->setPosition(182 / 2, 282 / 5 * 2); //  设置数字精灵的位置在卡牌水平居中，垂直2/5处
    _smallNumberSprite->setPosition(40, 282 - 40); //  设置小数字精灵的位置在卡牌左上角
    _suitSprite->setPosition(182 - 40, 282 - 40); //  设置花色精灵的位置在卡牌右上角
    _faceGeneralSprite->addChild(_numberSprite); //  将数字精灵添加到卡牌正面精灵中
    _faceGeneralSprite->addChild(_smallNumberSprite); //  将小数字精灵添加到卡牌正面精灵中
    _faceGeneralSprite->addChild(_suitSprite); //  将花色精灵添加到卡牌正面精灵中
    if (cardModel->flip) { //  根据卡牌翻转状态显示不同的面
        _faceGeneralSprite->setVisible(false); //  如果卡牌已翻转，隐藏正面精灵并设置水平缩放为0（实现翻转效果）
        _faceGeneralSprite->setScaleX(0);
        _backGeneralSprite->setVisible(true); //  显示背面精灵
    } else {
        _backGeneralSprite->setVisible(false); //  如果卡牌未翻转，隐藏背面精灵并设置水平缩放为0
        _backGeneralSprite->setScaleX(0);
        _faceGeneralSprite->setVisible(true); //  显示正面精灵
    }
    return true;
}

void CardView::setCardId(int cardId) {
    _cardId = cardId; //  将传入的卡牌ID赋值给成员变量_cardId
}

int CardView::getCardId() {
    return _cardId;
}

bool CardView::containsPoint(const cocos2d::Vec2& pos) {
    auto localPos = convertToNodeSpaceAR(pos); //  将世界坐标转换为节点本地坐标
    if (_faceGeneralSprite->isVisible()) //  如果卡片正面精灵可见
        return _faceGeneralSprite->getBoundingBox().containsPoint(localPos); //  检查点是否在正面精灵的边界框内
    if (_backGeneralSprite->isVisible()) //  如果卡片背面精灵可见
        return _backGeneralSprite->getBoundingBox().containsPoint(localPos); //  检查点是否在背面精灵的边界框内
    return false;
}


void CardView::setWorldPosition(cocos2d::Vec2 worldPosition) {
    _worldPosition = worldPosition; //  保存传入的世界坐标位置
}
cocos2d::Vec2 CardView::getWorldPosition() {
    return _worldPosition;
}

void CardView::playCardFlipAnimation() {
    if (_faceGeneralSprite->isVisible()) { //  如果卡片正面精灵可见
        _faceGeneralSprite->runAction( //  对卡牌正面精灵执行动作序列
            cocos2d::Sequence::create(
                cocos2d::ScaleTo::create(0.15f, 0, 1), //  创建一个缩放动画，将精灵在0.15秒内缩放到0宽度，保持原高度
                cocos2d::CallFunc::create([this](){ //  创建一个回调函数，在缩放动画完成后执行
                    _faceGeneralSprite->setVisible(false); //  隐藏正面精灵
                    _backGeneralSprite->setVisible(true); //  显示背面精灵
                    _backGeneralSprite->runAction(cocos2d::ScaleTo::create(0.15f, 1, 1)); //  对背面精灵执行缩放动画，在0.15秒内从0宽度恢复到原始大小
                }),
                nullptr
            ) //  结束ScaleTo动作创建
        );
    } else {
        _backGeneralSprite->runAction( //  如果条件不满足，则对背面精灵执行动画序列
            cocos2d::Sequence::create(
                cocos2d::ScaleTo::create(0.15f, 0, 1), //  创建一个缩放动画，将精灵在0.15秒内缩放到0宽度，保持原高度
                cocos2d::CallFunc::create([this](){ //  创建一个回调函数，在缩放动画完成后执行
                    _backGeneralSprite->setVisible(false); //  隐藏背面精灵
                    _faceGeneralSprite->setVisible(true); //  显示正面精灵
                    _faceGeneralSprite->runAction(cocos2d::ScaleTo::create(0.15f, 1, 1)); //  对正面精灵执行缩放动画，在0.15秒内从0宽度恢复到原始大小
                }),
                nullptr
            )
        );
    }
}
