#pragma once
#include "NoiseMap.h"
#include <Utils/Vector2.h>
#include <Utils/Random.h>
#include <vector>
#include <memory>
#include <array>
#include <string_view>

namespace yang
{
    class IGraphics;
    class IEvent;
    class ITexture;
    class Sprite;
}

enum class Biome : uint32_t
{
    kOcean,
    kBeach,
    kTundra,
    kTemperateSeasonalForest,
    kGrassland,
    kBorealForest,
    kShrubland,
    kTemperateRainforest,
    kTropicalRainForest,
    kSavanna,
    kSubtropicalDesert,
    kSnow,
    kMaxBiomes
};

class WorldMap
{
public:
    WorldMap() = default;
    ~WorldMap();
    bool Init(yang::IVec2 mapSize, yang::IVec2 tileSize, yang::FVec2 noiseSize, yang::IGraphics* pGraphics);
    bool Init(std::string_view pathToSettings);
    void Generate(uint64_t seed);
    bool Render(yang::IGraphics* pGraphics) const;
private:
    NoiseMap m_elevationMap;
    NoiseMap m_moistureMap;

    yang::IVec2 m_mapSize;
    yang::IVec2 m_tileSize;
    //yang::FVec2 m_tileScaleFactors;
    yang::FVec2 m_noiseSize;
    yang::XorshiftRNG m_rng;

    std::vector<Biome> m_biomes;
    std::array<std::shared_ptr<yang::Sprite>, static_cast<size_t>(Biome::kMaxBiomes)> m_biomeToSpriteArray;
    size_t m_keyboardListenerIndex;
    //size_t m_mouseListenerIndex;

    int m_numOctaves;
    float m_persistance;

    yang::IGraphics* m_pGraphics;
    yang::IVec2 m_center;

    void AssignBiomes(const std::vector<float>& elevations, const std::vector<float>& moistures);
    void AssignBiom(size_t index, float elevation, float moisture);
    Biome BiomeFromNoise(float elevation, float moisture);
    int GetIndexFromGridPoint(int x, int y) const;
    int GetIndexFromGridPoint(yang::IVec2 gridPoint) const;
    yang::IVec2 GetGridPointFromIndex(int index) const;
    float DistanceToCenter(int index) const;
    float GetRadiusSqrd() const;
    bool CheckBounds(int x, int y) const;
    
    std::shared_ptr<yang::Sprite> GetSpriteFromBiome(Biome biome) const;
    void HandleInputEvent(yang::IEvent* pEvent);
    void HandleWheelEvent(yang::IEvent* pEvent);
};