// services/CardMatchService.h
#ifndef _CARD_MATCH_SERVICE_H_
#define _CARD_MATCH_SERVICE_H_
#pragma once

#include "models/GameModel.h"
class CardMatchService {
public:
    static bool cardMatch(GameModel* gameModel, int CardId);
};

#endif