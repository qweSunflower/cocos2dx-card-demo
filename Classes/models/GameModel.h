// models/GameModel.h

#ifndef _GAME_MODEL_H_
#define _GAME_MODEL_H_
#pragma once

#include "models/CardModel.h"
#include <vector>

class GameModel {
public:
	std::vector<int> backupDesk;
	std::vector<int> cardDesk;
	std::vector<int> playfield;
	std::vector<CardModel*> cardModels;
	// void addCardToPlayfield(CardSuitType suit, CardFaceType face, cocos2d::Vec2 position) {
	// 	CardModel* card = CardModel::create(suit, face, position);
	// 	playfield.push_back(cardModels.size());
	// 	cardModels.push_back(card);
	// }
	// void addCardToBackupDesk(CardSuitType suit, CardFaceType face, cocos2d::Vec2 position) {
	// 	CardModel* card = CardModel::create(suit, face, position);
	// 	backupDesk.push(cardModels.size());
	// 	cardModels.push_back(card);
	// }
};


#endif // !_GAME_MODEL_H_