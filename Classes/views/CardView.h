// views/CardView.h
#ifndef _CARD_VIEW_H_
#define _CARD_VIEW_H_
#pragma once

#include "2d/CCNode.h"
#include "math/Vec2.h"
#include "2d/CCSprite.h"
#include "models/CardModel.h"
#include "cocos2d.h"

class CardView : public cocos2d::Node{
    cocos2d::Sprite* _faceGeneralSprite; //  面部精灵，用于显示卡牌正面的将军图像
    cocos2d::Sprite* _backGeneralSprite; //  背面精灵，用于显示卡牌背面的将军图像
    cocos2d::Sprite* _suitSprite; //  花色精灵，用于显示卡牌的花色图案
    cocos2d::Sprite* _numberSprite; //  数字精灵，用于显示卡牌的数字或点数
    cocos2d::Sprite* _smallNumberSprite;
    cocos2d::Vec2 _worldPosition;
    int _cardId;
public:
    static CardView* create(CardModel* cardModel);
    virtual bool initWithCardModel(CardModel* cardModel); //  创建游戏场景
    void setCardId(int cardId);
    int getCardId();
    void setWorldPosition(cocos2d::Vec2 worldPosition);
    cocos2d::Vec2 getWorldPosition();
    void setFlip(bool flip);
    bool containsPoint(const cocos2d::Vec2& pos);
    void playCardFlipAnimation();
};

#endif // !_CARD_VIEW_H_