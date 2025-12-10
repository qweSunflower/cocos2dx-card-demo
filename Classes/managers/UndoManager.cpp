#include "UndoManager.h"

UndoManager* UndoManager::create() {
    return new UndoManager;
}

void UndoManager::addRecord(bool isPlayField, cocos2d::Vec2 oldPosition) {
    _records.push_back({isPlayField, oldPosition});
}

UndoManager::Record UndoManager::popRecord() {
    auto record = _records.back();
    _records.pop_back();
    return record;
}


bool UndoManager::empty() {
    return _records.empty();
}