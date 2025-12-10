// services/CardMatchService.cpp
#include "CardMatchService.h"
#include <cstdlib>

bool CardMatchService::cardMatch(GameModel* gameModel, int CardId) {
    auto& target = gameModel->cardModels[gameModel->cardDesk.back()];
    auto& card = gameModel->cardModels[CardId];
    if (card->flip) return false;
    return abs(target->face - card->face) == 1;
}