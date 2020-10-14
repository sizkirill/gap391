#include "ForestGenerator.h"
#include "../CellularWorldMap.h"

ForestGenerator::ForestGenerator(int iterations, int neighborhoodSize, float percentage, ScoreFunc pScoreFunc)
    :Generator(iterations)
    ,m_neighborhoodSize(neighborhoodSize)
    ,m_percentage(percentage)
    ,m_pScoreFunc(pScoreFunc)
{
}

void ForestGenerator::Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng)
{
    if (currentTiles[static_cast<size_t>(index)] != TileStatus::kGrass)
    {
        buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        return;
    }

    int waterCount = owner.GetNeighborTileCount(index, TileStatus::kLake, m_neighborhoodSize);
    float score = m_pScoreFunc(waterCount) + m_percentage;

    if (rng.FRand<float>() < score)
    {
        buffer[static_cast<size_t>(index)] = TileStatus::kForest;
    }
    else
    {
        buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
    }
}
