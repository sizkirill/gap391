#pragma once
#include "Generator.h"

class ForestGenerator : public Generator
{
    using ScoreFunc = float(*)(int);
private:
    float m_percentage;
    int m_neighborhoodSize;
    ScoreFunc m_pScoreFunc;
public:
    ForestGenerator(int iterations, int neighborhoodSize, float percentage, ScoreFunc pScoreFunc);
    virtual void Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng) override final;

};