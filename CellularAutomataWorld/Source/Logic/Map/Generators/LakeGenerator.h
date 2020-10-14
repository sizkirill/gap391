#pragma once
#include "Generator.h"

class LakeGenerator : public Generator
{
private:
    int m_neighborhoodSize;
    int m_threshold;
    float m_percentage;
public:
    LakeGenerator(int iterations, int neighborhoodSize, int threshold, float percentage);
    virtual void Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng) override final;
};