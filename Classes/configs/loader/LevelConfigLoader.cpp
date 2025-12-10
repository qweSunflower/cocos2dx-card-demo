
#include "LevelConfigLoader.h"
#include "base/CCVector.h"
#include "base/ccUTF8.h"
#include "json/document.h"
#include <vector>
#include "configs/models/CardResConfig.h"
#include "configs/models/LevelConfig.h"
#include "math/Vec2.h"
#include "platform/CCFileUtils.h"

USING_NS_CC;

LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId) {
    auto path = StringUtils::format("res/levels/level_%d.json", levelId+1);
	std::string content = FileUtils::getInstance()->getStringFromFile(path);
	rapidjson::Document doc;
	doc.Parse(content.c_str());

    if (doc.HasParseError() || !doc.IsObject())  {
        CCLOG("LevelConfigLoader: JSON parse error for %s", path.c_str());
        throw std::runtime_error("JSON parse error");
    }
    auto levelConfig = new LevelConfig();
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const auto& arr = doc["Playfield"];
        for (auto& item : arr.GetArray()) {
            std::vector<int> downCards;
            if (item.HasMember("DownCards") && item["DownCards"].IsArray()) {
                for (auto& index : item["DownCards"].GetArray()) {
                    downCards.push_back(index.GetInt());
                }
            }
            levelConfig->playfield.emplace_back(static_cast<CardFaceType>(item["CardFace"].GetInt()), static_cast<CardSuitType>(item["CardSuit"].GetInt()), cocos2d::Vec2{item["Position"]["x"].GetFloat(), item["Position"]["y"].GetFloat()}, downCards);
        }
    }
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const auto& arr = doc["Stack"];
        for (auto& item : arr.GetArray()) {
            levelConfig->stack.emplace_back(static_cast<CardFaceType>(item["CardFace"].GetInt()), static_cast<CardSuitType>(item["CardSuit"].GetInt()), cocos2d::Vec2{item["Position"]["x"].GetFloat(), item["Position"]["y"].GetFloat()}, std::vector<int>());
        }
    }
    return levelConfig;
}