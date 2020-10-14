#include "ForestPropagator.h"
#include <Logic/Map/CellularWorldMap.h>
#include <cmath>

ForestPropagator::ForestPropagator(int iterations, ScoreFuncWrapper pWaterScoreFunc, ScoreFuncWrapper pForestScoreFunc, const std::vector<float>& moistureMap)
    :Generator(iterations)
    ,m_pNearbyWaterScoreFunc(pWaterScoreFunc)
    ,m_pNearbyForestScoreFunc(pForestScoreFunc)
    ,m_moistureMap(moistureMap)
{
}

void ForestPropagator::Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng)
{
    if (currentTiles[static_cast<size_t>(index)] != TileStatus::kGrass)
    {
        buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        return;
    }

    int forestCount = owner.GetNeighborTileCount(index, TileStatus::kForest, m_pNearbyForestScoreFunc.m_neighborhoodSize);
    float forestScore = m_pNearbyForestScoreFunc.m_scoreFunc(forestCount);

    // yes, if it is exactly zero
    // if there is no forest around, don't make a new one
    if (forestScore == 0.f)
    {
        buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        return;
    }

    int waterCount = owner.GetNeighborTileCount(index, TileStatus::kLake, m_pNearbyWaterScoreFunc.m_neighborhoodSize);
    float waterScore = m_pNearbyWaterScoreFunc.m_scoreFunc(waterCount);

    // for future moisture
    float moistureVal = m_moistureMap[index] / 2.f;

    // if waterscore is zero we still want a chance to propagate, especially if there is a forest here already, so using arithmetic mean
    float totalScore = (forestScore + waterScore) / 2.f;
    totalScore = std::sqrtf(totalScore * moistureVal);


    if (totalScore > rng.FRand<float>())
    {
        buffer[static_cast<size_t>(index)] = TileStatus::kForest;
    }
    else
    {
        buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
    }
}
