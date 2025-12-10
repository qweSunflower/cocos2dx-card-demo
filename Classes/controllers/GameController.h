// controllers/GameController.h
#ifndef _GAME_CONTROLLER_H_
#define _GAME_CONTROLLER_H_
#include "managers/UndoManager.h"
#pragma once

#include "base/CCRef.h"
#include "views/CardView.h"
#include "views/GameView.h"
#include "models/GameModel.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"

class GameController {
	GameView* _gameView;
	GameModel* _gameModel;
	UndoManager* _undoManager;
	StackController* _stackController;
	PlayFieldController* _playFieldController;
public:
	static GameController* create();
	void startGame(int levelId);
	void initView(GameModel* gameModel);
	void handleBackButtonClick(cocos2d::Ref* sender);
	void handlePlayFieldMoveToStack(int cardId);
	void handleStackMoveToUsed();
};

#endif // !_GAME_CONTROLLER_H_
