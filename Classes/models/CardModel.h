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
	CardSuitType suit;
	CardFaceType face;
	cocos2d::Vec2 postion;
	std::vector<CardModel*> upCards;
	int upCardNum;
	std::vector<int> downCards;
	bool flip;
	CardModel(CardSuitType suit, CardFaceType face, cocos2d::Vec2 position, std::vector<int> downCards);
};

#endif // !_CARD_MODEL_H_
