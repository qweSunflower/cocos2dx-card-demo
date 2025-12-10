// configs/models/LevelConfig.h
#ifndef _LEVEL_CONFIG_H_
#define _LEVEL_CONFIG_H_
#pragma once

#include <vector>
#include "cocos2d.h"
#include "configs/models/CardResConfig.h"

class LevelConfig {
public:
	struct CardConfig {
		CardFaceType face;
		CardSuitType suit;
		cocos2d::Vec2 position;
		std::vector<int> dowCards;
		CardConfig (CardFaceType f, CardSuitType s, cocos2d::Vec2 pos, std::vector<int> downCards) : face(f), suit(s), position(pos), dowCards(downCards) {}
	};
	std::vector<CardConfig> playfield;
	std::vector<CardConfig> stack;
};

#endif