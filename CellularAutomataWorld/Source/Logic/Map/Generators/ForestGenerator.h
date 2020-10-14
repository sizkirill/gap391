#pragma once
#include "Generator.h"

class ForestGenerator : public Generator
{
    using ScoreFunc = float(*)(int);
private:
    const std::vector<float>& m_moistureMap;
    int m_neighborhoodSize;
    ScoreFunc m_pScoreFunc;
public:
    ForestGenerator(int iterations, int neighborhoodSize, const std::vector<float>& moistureMap, ScoreFunc pScoreFunc);
    virtual void Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng) override final;

};