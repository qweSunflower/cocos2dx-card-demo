
#include "GameModelFromLevelGenerator.h"
#include "models/CardModel.h"
#include "models/GameModel.h"
#include <vector>

GameModel* GameModelFromLevelGenerator::generateGameModel(LevelConfig* levelConfig) {
	auto gameModel = new GameModel();
	std::map<int, int> downCards;
	for (const auto& cardConfig : levelConfig->playfield) {
		CardModel* card = CardModel::create(cardConfig.suit, cardConfig.face, cardConfig.position, cardConfig.dowCards);
		for (auto i : cardConfig.dowCards) {
			if (downCards.find(i) == downCards.end()) {
				downCards[i] = 0;
			}
			downCards[i] ++;
		}
		gameModel->playfield.push_back(gameModel->cardModels.size());
		gameModel->cardModels.push_back(card);
	}
	for (auto i : downCards) {
		gameModel->cardModels[i.first]->flip = true;
		gameModel->cardModels[i.first]->upCardNum = i.second;
	}
	for (const auto& cardConfig : levelConfig->stack) {
		CardModel* card = CardModel::create(cardConfig.suit, cardConfig.face, cardConfig.position, cardConfig.dowCards);
		card->flip = true;
		gameModel->backupDesk.push_back(gameModel->cardModels.size());
		gameModel->cardModels.push_back(card);
	}
	gameModel->cardDesk.push_back(gameModel->backupDesk.back());
	gameModel->backupDesk.pop_back();
	gameModel->cardModels[gameModel->cardDesk.back()]->flip = false;
	return gameModel;
}