#include "CardView.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionInterval.h"
#include "configs/models/CardResConfig.h"
#include "math/Vec2.h"
#include "models/CardModel.h"

CardView* CardView::create(CardModel* cardModel) {
    CardView* ret = new (std::nothrow) CardView();
    if (ret && ret->initWithCardModel(cardModel)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CardView::initWithCardModel(CardModel* cardModel) {
    setPosition(cardModel->postion);
    _faceGeneralSprite = cocos2d::Sprite::create(CardResConfig::faceGeneralPath);
    _backGeneralSprite = cocos2d::Sprite::create(CardResConfig::backGeneralPath);
    addChild(_faceGeneralSprite);
    addChild(_backGeneralSprite);
    const auto& cardResConfig = CardResConfig::getCardResConfig(cardModel->suit, cardModel->face);
    _numberSprite = cocos2d::Sprite::create(cardResConfig->numberPath);
    _smallNumberSprite = cocos2d::Sprite::create(cardResConfig->smallNumberPath);
    _suitSprite = cocos2d::Sprite::create(cardResConfig->suitPath);
    // _faceGeneralSprite->setPosition(cardModel->postion);
    _numberSprite->setPosition(182 / 2, 282 / 5 * 2);
    _smallNumberSprite->setPosition(40, 282 - 40);
    _suitSprite->setPosition(182 - 40, 282 - 40);
    _faceGeneralSprite->addChild(_numberSprite);
    _faceGeneralSprite->addChild(_smallNumberSprite);
    _faceGeneralSprite->addChild(_suitSprite);
    if (cardModel->flip) {
        _faceGeneralSprite->setVisible(false);
        _faceGeneralSprite->setScaleX(0);
        _backGeneralSprite->setVisible(true);
    } else {
        _backGeneralSprite->setVisible(false);
        _backGeneralSprite->setScaleX(0);
        _faceGeneralSprite->setVisible(true);
    }
    return true;
}

void CardView::setCardId(int cardId) {
    _cardId = cardId;
}

int CardView::getCardId() {
    return _cardId;
}

bool CardView::containsPoint(const cocos2d::Vec2& pos) {
    auto localPos = convertToNodeSpaceAR(pos);
    if (_faceGeneralSprite->isVisible())
        return _faceGeneralSprite->getBoundingBox().containsPoint(localPos);
    if (_backGeneralSprite->isVisible())
        return _backGeneralSprite->getBoundingBox().containsPoint(localPos);
}


void CardView::setWorldPosition(cocos2d::Vec2 worldPosition) {
    _worldPosition = worldPosition;
}
cocos2d::Vec2 CardView::getWorldPosition() {
    return _worldPosition;
}


void CardView::playCardFlipAnimation() {
    if (_faceGeneralSprite->isVisible()) {
        _faceGeneralSprite->runAction(
            cocos2d::Sequence::create(
                cocos2d::ScaleTo::create(0.15f, 0, 1),
                cocos2d::CallFunc::create([this](){
                    _faceGeneralSprite->setVisible(false);
                    _backGeneralSprite->setVisible(true);
                    _backGeneralSprite->runAction(cocos2d::ScaleTo::create(0.15f, 1, 1));
                }),
                nullptr
            )
        );
    } else {
        _backGeneralSprite->runAction(
            cocos2d::Sequence::create(
                cocos2d::ScaleTo::create(0.15f, 0, 1),
                cocos2d::CallFunc::create([this](){
                    _backGeneralSprite->setVisible(false);
                    _faceGeneralSprite->setVisible(true);
                    _faceGeneralSprite->runAction(cocos2d::ScaleTo::create(0.15f, 1, 1));
                }),
                nullptr
            )
        );
    }
}
