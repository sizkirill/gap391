#pragma once
#include <vector>
#include "../TileStatus.h"
#include <Utils/Random.h>

class CellularWorldMap;

class Generator
{
private:
    int m_numIterations;
public:
    Generator(int iterations) :m_numIterations(iterations) {}
    virtual void Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng) = 0;
    void Decrement() { --m_numIterations; }
    int Iterations() const { return m_numIterations; }
};