// managers/UndoManager.h
#ifndef _UNDO_MANAGER_H_
#define _UNDO_MANAGER_H_
#include "math/Vec2.h"
#include <vector>
#pragma once

class UndoManager {
    struct Record {
        bool isPlayField;
        cocos2d::Vec2 oldPosition;
    };
    std::vector<Record> _records;
public:
    static UndoManager* create();
    void addRecord(bool isPlayField, cocos2d::Vec2 oldPosition);
    Record popRecord();
    bool empty();
};

#endif // !_UNDO_MANAGER_H_