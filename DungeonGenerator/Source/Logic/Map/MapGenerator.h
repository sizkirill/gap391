#pragma once
#include <Utils/Random.h>
#include <Utils/Vector2.h>
#include "DungeonMap.h"
#include <string_view>
#include <unordered_map>
#include <Application/Graphics/Textures/Sprite.h>

class MapGenerator
{
public:
    MapGenerator() = default;
    MapGenerator(uint64_t seed);

    bool Init(std::string_view pathToSettings);

    DungeonMap GenerateMap();
private:
    bool IsDone() const;
private:

    static constexpr int kDesiredRoomCount = 16;

    /// <summary>
    /// Room default properties
    /// </summary>
    static constexpr yang::IVec2 kDefaultRoomHeightRange = { 3,7 };
    static constexpr yang::IVec2 kDefaultRoomWidthRange = { 3,7 };

    /// <summary>
    /// Default map properties
    /// </summary>
    static constexpr yang::IVec2 kDefaultMapSize = { 100,100 };
    static constexpr yang::IVec2 kDefaultTileSize = { 16,16 };
    static constexpr yang::IVec2 kDefaultStartPos = kDefaultMapSize / 2;

    yang::XorshiftRNG m_rngdevice;    ///< RNG seed for this gen

    int m_desiredRoomCount = kDesiredRoomCount;
    int m_currentRoomCount = 0;

    yang::IVec2 m_roomWidthRange = kDefaultRoomWidthRange;
    yang::IVec2 m_roomHeightRange = kDefaultRoomHeightRange;

    yang::IVec2 m_startPos = kDefaultStartPos;

    yang::IVec2 m_mapSize = kDefaultMapSize;
    yang::IVec2 m_tileSize = kDefaultTileSize;
    std::unordered_map<uint32_t, std::shared_ptr<yang::Sprite>> m_tileSpriteMap;
};