// models/CardModel.h
#ifndef _CARD_MODEL_H_
#define _CARD_MODEL_H_
#include <vector>
#pragma once

#include "configs/models/CardResConfig.h"
#include "cocos2d.h"

class CardModel {
public:
	static CardModel* create(CardSuitType suit, CardFaceType face, cocos2d::Vec2 position, std::vector<int> downCards);
	CardSuitType suit;  // 花色
	CardFaceType face;  // 点数
	cocos2d::Vec2 postion;  // 位置
	int upCardNum;  // 上牌数量
	std::vector<int> downCards;  // 下牌indexs
	bool flip;  // 是否翻转
	CardModel(CardSuitType suit, CardFaceType face, cocos2d::Vec2 position, std::vector<int> downCards);
};

#endif // !_CARD_MODEL_H_
