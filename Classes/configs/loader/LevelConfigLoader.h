// configs/loaders/LevelConfigLoader.h
#ifndef _LEVEL_CONFIG_LOADER_H_
#define _LEVEL_CONFIG_LOADER_H_
#pragma once

#include "configs/models/LevelConfig.h"

class LevelConfigLoader {
public:
	static LevelConfig* loadLevelConfig(int levelId);
};

#endif // !_LEVEL_CONFIG_LOADER_H_
