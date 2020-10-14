#pragma once
#include <Utils/Vector2.h>
#include <vector>
#include <Utils/Random.h>

namespace yang
{
    class IGraphics;
}

class NoiseMap
{
public:
    NoiseMap() = default;
    bool Init(yang::IVec2 mapSize, yang::IVec2 tileSize, yang::FVec2 noiseSize);
    std::vector<float> Generate(uint32_t seed = 0x1b56c4e9, int numOctaves = 4, float persistance = 0.5f);
    bool Render(yang::IGraphics* pGraphics) const;
private:
    yang::IVec2 m_mapSize;
    yang::IVec2 m_tileSize;

    yang::FVec2 m_noiseSize;

    float m_noiseMin;
    float m_noiseMax;

    yang::FVec2 GetNoiseCoordsFromGridPoint(int x, int y) const;
    yang::FVec2 GetNoiseCoordsFromGridPoint(yang::IVec2 point) const;
    int GetIndexFromGridPoint(int x, int y) const;
    int GetIndexFromGridPoint(yang::IVec2 gridPoint) const;
    yang::IVec2 GetGridPointFromIndex(int index) const;
};