#pragma once
#include "Generator.h"

class VillageGenerator : public Generator
{
    using ScoreFunc = float(*)(int);
private:
    int m_waterNeighborhoodSize;
    int m_villageNeighborhoodSize;
    ScoreFunc m_pWaterScoreFunc;
    ScoreFunc m_pVillageScoreFunc;
public:
    VillageGenerator(int iterations, int waterNeighborhoodSize, int villageNeighborhoodSize, ScoreFunc pWaterFunc, ScoreFunc pVillageFunc);
    virtual void Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng) override final;
};