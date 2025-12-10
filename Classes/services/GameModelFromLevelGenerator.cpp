
#include "GameModelFromLevelGenerator.h"
#include "models/CardModel.h"
#include "models/GameModel.h"
#include <vector>

GameModel* GameModelFromLevelGenerator::generateGameModel(LevelConfig* levelConfig) {
	auto gameModel = new GameModel(); //  创建新的游戏模型实例
	std::map<int, int> downCards; //  用于记录下方卡片的映射表，键为卡片ID，值为计数
	for (const auto& cardConfig : levelConfig->playfield) { //  遍历关卡配置中的游戏场地配置
		CardModel* card = CardModel::create(cardConfig.suit, cardConfig.face, cardConfig.position, cardConfig.dowCards); //  根据卡片配置创建卡片模型
		for (auto i : cardConfig.dowCards) { //  遍历当前卡片的下方卡片列表
			if (downCards.find(i) == downCards.end()) { //  如果下方卡片ID尚未在映射表中，则初始化计数为0
				downCards[i] = 0;
			}
			downCards[i] ++; //  增加对应下方卡片的计数
		}
		gameModel->playfield.push_back(gameModel->cardModels.size()); //  将当前卡片模型的数量添加到游戏模型的游戏区域中
		gameModel->cardModels.push_back(card); //  将当前卡片添加到游戏模型的卡片模型列表中
	}
	for (auto i : downCards) {
		gameModel->cardModels[i.first]->flip = true; //  设置指定索引的卡片模型的翻转状态为true
		gameModel->cardModels[i.first]->upCardNum = i.second; //  设置指定索引的卡片模型的上面卡片数目
	}
	for (const auto& cardConfig : levelConfig->stack) { //  遍历关卡配置中的堆栈配置
		CardModel* card = CardModel::create(cardConfig.suit, cardConfig.face, cardConfig.position, cardConfig.dowCards); //  创建一个新的卡片模型
		card->flip = true; //  设置卡片模型的翻转状态为true
		gameModel->backupDesk.push_back(gameModel->cardModels.size()); //  将当前卡片模型的数量添加到游戏模型的备份桌面中
		gameModel->cardModels.push_back(card); //  将当前卡片模型添加到游戏模型的卡片模型列表中
	}
	gameModel->cardDesk.push_back(gameModel->backupDesk.back()); //  将备份堆的最后一个元素添加到游戏模型的卡片堆中
	gameModel->backupDesk.pop_back(); //  移除备份堆的最后一个元素
	gameModel->cardModels[gameModel->cardDesk.back()]->flip = false; //  设置卡片桌面中最后一个元素的卡片模型的翻转状态为false
	return gameModel; //  返回创建的游戏模型
}