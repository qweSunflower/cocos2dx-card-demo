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
};


#endif // !_GAME_MODEL_H_