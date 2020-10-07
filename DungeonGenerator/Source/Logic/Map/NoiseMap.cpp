#include "NoiseMap.h"
#include <Application/Graphics/IGraphics.h>
#include <Utils/Color.h>
#include <Utils/Logger.h>
#include <Utils/PerlinNoise.h>
#include <chrono>
#include <execution>
#include <thread>

bool NoiseMap::Init(yang::IVec2 mapSize, yang::IVec2 tileSize, yang::FVec2 noiseSize)
{
    m_mapSize = mapSize;
    m_tileSize = tileSize;
    m_noiseSize = noiseSize;
    return true;
}

std::vector<float> NoiseMap::Generate(uint32_t seed, int numOctaves, float persistance)
{
    yang::PerlinNoise noise(seed);
    std::vector<float> values(m_mapSize.x * m_mapSize.y);

    auto start = std::chrono::steady_clock::now();

    static constexpr size_t kNumThreads = 8;
    std::thread jobs[kNumThreads];
    size_t jobSize = values.size() / kNumThreads;
    for (size_t i = 0; i < kNumThreads; ++i)
    {
        size_t startIndex = i * jobSize;
        size_t endIndex = (i == kNumThreads - 1 ? values.size() : (i + 1) * jobSize);
        jobs[i] = std::thread([startIndex, endIndex, &values, &noise, this, numOctaves, persistance]()
            {
                for (size_t i = startIndex; i < endIndex; ++i)
                {
                    values[i] = noise.NormalizedNoise(static_cast<int>(i) % m_mapSize.x, static_cast<int>(i) / m_mapSize.x, m_mapSize.x, m_mapSize.y, m_noiseSize, numOctaves, persistance);
                }
            });
    }

    for (auto& t : jobs)
    {
        t.join();
    }

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<float> duration = end - start;
    LOG(Info, "Perlin noise took %f seconds", duration.count());

    m_noiseMin = noise.GetMin();
    m_noiseMax = noise.GetMax();
    
    std::for_each(values.begin(), values.end(), [this, &noise](float& val) {val = noise.NormalizationFunc()(val); });

    auto it = std::find_if(values.begin(), values.end(), [](float val) {return val < 0 || val > 1.f; });
    if (it != values.end())
    {
        LOG(Error, "Value was less than zero or greater than 1: %f, index: %d", *it, it - values.begin());
    }

    return values;
}

bool NoiseMap::Render(yang::IGraphics* pGraphics) const
{
    return true;
}

yang::FVec2 NoiseMap::GetNoiseCoordsFromGridPoint(int x, int y) const
{
    float fx = ((static_cast<float>(x) + 0.5f) / m_mapSize.x) * m_noiseSize.x;
    float fy = ((static_cast<float>(y) + 0.5f) / m_mapSize.y) * m_noiseSize.y;
    return { fx, fy };
}

yang::FVec2 NoiseMap::GetNoiseCoordsFromGridPoint(yang::IVec2 point) const
{
    return GetNoiseCoordsFromGridPoint(point.x, point.y);
}

int NoiseMap::GetIndexFromGridPoint(int x, int y) const
{
    return y * m_mapSize.x + x;
}

int NoiseMap::GetIndexFromGridPoint(yang::IVec2 gridPoint) const
{
    return GetIndexFromGridPoint(gridPoint.x, gridPoint.y);
}

yang::IVec2 NoiseMap::GetGridPointFromIndex(int index) const
{
    return {index % m_mapSize.x, index / m_mapSize.x};
}
