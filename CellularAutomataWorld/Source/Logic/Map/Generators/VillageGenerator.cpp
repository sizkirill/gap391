#include "VillageGenerator.h"
#include <Logic/Map/CellularWorldMap.h>

VillageGenerator::VillageGenerator(int iterations, int waterNeighborhoodSize, int villageNeighborhoodSize, ScoreFunc pWaterFunc, ScoreFunc pVillageFunc)
    :Generator(iterations)
    ,m_waterNeighborhoodSize(waterNeighborhoodSize)
    ,m_villageNeighborhoodSize(villageNeighborhoodSize)
    ,m_pWaterScoreFunc(pWaterFunc)
    ,m_pVillageScoreFunc(pVillageFunc)
{
}

// Some actually random value to generate whatever is happening
void VillageGenerator::Propagate(int index, const std::vector<TileStatus>& currentTiles, std::vector<TileStatus>& buffer, const CellularWorldMap& owner, yang::XorshiftRNG& rng)
{
    if (currentTiles[static_cast<size_t>(index)] != TileStatus::kGrass && currentTiles[static_cast<size_t>(index)] != TileStatus::kVillage)
    {
        buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        return;
    }

    int waterCount = owner.GetNeighborTileCount(index, TileStatus::kLake, m_waterNeighborhoodSize);
    int villageCount = owner.GetNeighborTileCount(index, TileStatus::kVillage, m_villageNeighborhoodSize);
    float waterScore = m_pWaterScoreFunc(waterCount);
    float villageScore = m_pVillageScoreFunc(villageCount);

    if (villageCount > 0 && waterCount > 0)
    {
        float finalScore = std::sqrtf(waterScore * villageScore);

        if (currentTiles[static_cast<size_t>(index)] == TileStatus::kVillage && finalScore < 0.15f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kGrass;
        }
        else if (finalScore > 0.5f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kVillage;
        }
    }
    else if (villageCount > 0)
    {
        // if there is a village and no water slight chance to delete it
        if (currentTiles[static_cast<size_t>(index)] == TileStatus::kVillage && rng.FRand<float>() * villageScore < 0.001f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kGrass;
        }
        // and a chance to grow it
        else if (rng.FRand<float>() * villageScore > 0.2f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kVillage;
        }
        else
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kGrass;
        }
    }
    else if (waterCount > 0)
    {
        // chance to spawn village if there is water arounf
        float finalScore = waterScore / 400.f;

        if (rng.FRand<float>() < finalScore)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kVillage;
        }
        else
        {
            buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        }
    }
    else
    {
        // Small chance to spawn village in a middle of nowhere
        if (rng.FRand<float>() < 0.00001f)
        {
            buffer[static_cast<size_t>(index)] = TileStatus::kVillage;
        }
        else
        {
            buffer[static_cast<size_t>(index)] = currentTiles[static_cast<size_t>(index)];
        }
    }
}
