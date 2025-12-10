// servies/GameModelFromLevelGenerator.h
#ifndef _GAME_MODEL_FROM_LEVEL_GENERATOR_H_
#define _GAME_MODEL_FROM_LEVEL_GENERATOR_H_
#pragma once

#include <memory>
#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"

class GameModelFromLevelGenerator {
public:
	static GameModel* generateGameModel(LevelConfig* levelConfig);
};

#endif // !_GAME_MODEL_FROM_LEVEL_GENERATOR_H_
