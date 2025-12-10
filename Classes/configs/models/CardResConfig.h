// configs/models/CardResConfig.h
#ifndef _CARD_RES_CONFIG_H_
#define _CARD_RES_CONFIG_H_

#include <map>
#include <utility>
#include <vector>
#pragma once

#include <string>

// ��ɫ����
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // ÷��
    CST_DIAMONDS,   // ����
    CST_HEARTS,     // ����
    CST_SPADES,     // ����
    CST_NUM_CARD_SUIT_TYPES
};

// ��������
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

class CardResConfig {
    static std::map<std::pair<CardSuitType, CardFaceType>, CardResConfig*> _cardResConfigs;
    static const std::vector<const char*> _faceNames;
    static const std::vector<const char*> _suitPaths;
public:
    static const CardResConfig* getCardResConfig(CardSuitType suit, CardFaceType face);
    static const char* const faceGeneralPath;
    static const char* const backGeneralPath;
    std::string numberPath;
    std::string smallNumberPath;
    std::string suitPath;
};

#endif