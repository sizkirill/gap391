#pragma once
#include "Generator.h"
#include <vector>

class RockGenerator : public Generator
{
private:
    int m_rockNeighborhoodSize;
    int m_iceNeighborhoodSize;
    const std::vector<float>& m_moistureMap;
    const std::vector<float>& m_elevationMap;
public:
    RockGenerator(int iterations, int iceNeighborhoodSize, int rockNeighborhoodSize, const std::vector<float>& moistureMap, const std::vector<float>& elevationMap);
    virtual void Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng) override final;
};