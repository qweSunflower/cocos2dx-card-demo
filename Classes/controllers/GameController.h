// controllers/GameController.h
#ifndef _GAME_CONTROLLER_H_
#define _GAME_CONTROLLER_H_
#pragma once

#include "base/CCRef.h"
#include "views/CardView.h"
#include "managers/UndoManager.h"
#include "views/GameView.h"
#include "models/GameModel.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"

class GameController {
	GameView* _gameView; //  游戏视图指针，负责显示游戏界面
	GameModel* _gameModel; //  游戏模型指针，负责存储游戏数据
	UndoManager* _undoManager; //  撤销管理器指针，负责处理游戏撤销操作
	StackController* _stackController; //  牌堆控制器指针，负责管理牌堆相关操作
	PlayFieldController* _playFieldController; //  游戏区域控制器指针，负责管理游戏区域操作
public:
	static GameController* create();
	void startGame(int levelId);
	void initView(GameModel* gameModel);
	void handleBackButtonClick(cocos2d::Ref* sender);
	void handlePlayFieldMoveToStack(int cardId);
	void handleStackMoveToUsed();
};

#endif // !_GAME_CONTROLLER_H_
