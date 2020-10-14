#pragma once
#include "Generator.h"
#include <vector>

class LakeGenerator : public Generator
{
private:
    int m_neighborhoodSize;
    int m_threshold;
    const std::vector<float>& m_moistureMap;
public:
    LakeGenerator(int iterations, int neighborhoodSize, int threshold, const std::vector<float>& moistureMap);
    virtual void Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng) override final;
};