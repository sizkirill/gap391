#include "LakeGenerator.h"
#include <Logic/Map/CellularWorldMap.h>

LakeGenerator::LakeGenerator(int iterations, int neighborhoodSize, int threshold, const std::vector<float>& moistureMap)
    :Generator(iterations)
    ,m_neighborhoodSize(neighborhoodSize)
    ,m_threshold(threshold)
    ,m_moistureMap(moistureMap)
{
}

void LakeGenerator::Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng)
{
    int waterCount = owner.GetNeighborTileCount(index, TileStatus::kLake, m_neighborhoodSize);
    if (waterCount >= 6)
    {
        buffer[static_cast<size_t>(index)] = TileStatus::kLake;
    }
    else if (waterCount >= m_threshold && rng.FRand<float>() < m_moistureMap[index] * 0.5f)
    {
        buffer[static_cast<size_t>(index)] = TileStatus::kLake;
    }
    else
    {
        buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
    }
}
