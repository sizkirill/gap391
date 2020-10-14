#include "RockGenerator.h"
#include <Logic/Map/CellularWorldMap.h>
#include <Utils/Math.h>

RockGenerator::RockGenerator(int iterations, int iceNeighborhoodSize, int rockNeighborhoodSize, const std::vector<float>& moistureMap, const std::vector<float>& elevationMap)
    :Generator(iterations)
    ,m_iceNeighborhoodSize(iceNeighborhoodSize)
    ,m_rockNeighborhoodSize(rockNeighborhoodSize)
    ,m_moistureMap(moistureMap)
    ,m_elevationMap(elevationMap)
{
}

void RockGenerator::Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng)
{
    if (currentTiles[static_cast<size_t>(index)] != TileStatus::kGrass)
    {
        buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        return;
    }

    int iceCount = owner.GetNeighborTileCount(index, TileStatus::kIce, m_iceNeighborhoodSize);
    int rockCount = owner.GetNeighborTileCount(index, TileStatus::kRock, m_rockNeighborhoodSize);
    float iceScore = Math::SmootherStep(std::sqrtf(m_elevationMap[index] * m_moistureMap[index]));
    float rockScore = Math::SmootherStep(std::sqrtf(m_elevationMap[index] * (1 - m_moistureMap[index])));

    if (iceCount > 0 && rockCount > 0)
    {
        iceScore = iceScore * (static_cast<float>(iceCount) / (m_iceNeighborhoodSize * m_iceNeighborhoodSize));
        rockScore = rockScore * (static_cast<float>(rockCount) / (m_rockNeighborhoodSize * m_rockNeighborhoodSize));

        if (iceScore > 0.4f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kIce;
        }
        else if (iceScore > 0.2f && rockScore > 0.3f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kRock;
        }
        else
        {
            buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        }
    }
    else if (iceCount > 0)
    {
        iceScore = iceScore * (static_cast<float>(iceCount) / (m_iceNeighborhoodSize * m_iceNeighborhoodSize));
        if (iceScore > 0.95f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kIce;
        }
        else if (rockScore > 0.35f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kRock;
        }
        else
        {
            buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        }
    }
    else if (rockCount > 0)
    {
        rockScore = rockScore * (static_cast<float>(rockCount) / (m_rockNeighborhoodSize * m_rockNeighborhoodSize));
        if (iceScore > 0.65f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kIce;
        }
        else if (rockScore > 0.9f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kRock;
        }
        else
        {
            buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        }
    }
    else
    {
        if (iceScore > 0.92f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kIce;
        }
        else if (rockScore > 0.92f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kRock;
        }
        else
        {
            buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        }
    }
}
