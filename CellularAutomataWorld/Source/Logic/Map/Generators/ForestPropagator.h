#pragma once
#include "Generator.h"

struct ScoreFuncWrapper
{
    using ScoreFunc = float(*)(int);

    int m_neighborhoodSize = 1;
    ScoreFunc m_scoreFunc = nullptr;
};

class ForestPropagator : public Generator
{
    
private:
    ScoreFuncWrapper m_pNearbyWaterScoreFunc;
    ScoreFuncWrapper m_pNearbyForestScoreFunc;
    const std::vector<float>& m_moistureMap;
public:
    ForestPropagator(int iterations, ScoreFuncWrapper pWaterScoreFunc, ScoreFuncWrapper pForestScoreFunc, const std::vector<float>& moistureMap);
    virtual void Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng) override final;

};