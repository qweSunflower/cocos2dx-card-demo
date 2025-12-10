// models/CardModel.cpp
#include "CardModel.h"

CardModel::CardModel(CardSuitType suit, CardFaceType face, cocos2d::Vec2 position, std::vector<int> downCards)
    : suit(suit), face(face), postion(position), downCards(downCards), flip(false) {
}

CardModel* CardModel::create(CardSuitType suit, CardFaceType face, cocos2d::Vec2 position, std::vector<int> downCards) {
    return new CardModel(suit, face, position, downCards);
}