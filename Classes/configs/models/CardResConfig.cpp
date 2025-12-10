// configs/models/CardResConfig.cpp
#include "CardResConfig.h"
#include "base/ccUTF8.h"

std::map<std::pair<CardSuitType, CardFaceType>, CardResConfig*> CardResConfig::_cardResConfigs;

const std::vector<const char*> CardResConfig::_faceNames = {
    "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
};

const std::vector<const char*> CardResConfig::_suitPaths = {
    "res/suits/club.png", "res/suits/diamond.png", "res/suits/heart.png", "res/suits/spade.png"
};

const char* const CardResConfig::faceGeneralPath = "res/card_general.png";

const char* const CardResConfig::backGeneralPath = "res/card_general.png";

const CardResConfig* CardResConfig::getCardResConfig(CardSuitType suit, CardFaceType face) {
    auto key = std::make_pair(suit, face);
    auto it = CardResConfig::_cardResConfigs.find(key);
    if (it != CardResConfig::_cardResConfigs.end()) {
        return (it->second);
    }
    auto config = new CardResConfig();
    bool isRedSuit = (suit == CST_HEARTS || suit == CST_DIAMONDS);
    const char* colorStr = isRedSuit ? "red" : "black";
    config->numberPath = cocos2d::StringUtils::format("res/number/big_%s_%s.png", colorStr, _faceNames[face]);
    config->smallNumberPath = cocos2d::StringUtils::format("res/number/small_%s_%s.png", colorStr, _faceNames[face]);
    config->suitPath = _suitPaths[suit];

    CardResConfig::_cardResConfigs[key] = config;
    return config;
}