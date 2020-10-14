#include "PerlinNoise.h"
#include <cmath>

yang::PerlinNoise::PerlinNoise(uint32_t seed)
    :m_seed(seed)
    ,m_rng()
    ,m_min(0)
    ,m_max(0)
{
}

float yang::PerlinNoise::GetNoise(float x, float y)
{
    //if (finalNoise > m_max) m_max = finalNoise;
    //if (finalNoise < m_min) m_min = finalNoise;

    return GetNoise(x, y, m_seed);
}

float yang::PerlinNoise::GetNoise(yang::FVec2 coords)
{
    return GetNoise(coords.x, coords.y);
}

float yang::PerlinNoise::GetNoise(int x, int y, int maxX, int maxY, FVec2 inputRange)
{
    float noiseGridX = ((static_cast<float>(x) + 0.5f) / maxX) * inputRange.x;
    float noiseGridY = ((static_cast<float>(y) + 0.5f) / maxY) * inputRange.y;
    return GetNoise(noiseGridX, noiseGridY);
}

float yang::PerlinNoise::GetNoise(IVec2 coords, IVec2 maxCoords, FVec2 inputRange)
{
    return GetNoise(coords.x, coords.y, maxCoords.x, maxCoords.y, inputRange);
}

float yang::PerlinNoise::NormalizedNoise(int x, int y, int maxX, int maxY, FVec2 inputRange, int numOctaves, float persistance)
{
    if (numOctaves <= 0)
        return 0.f;

    float noise = 0;
    float currentAmplitude = 1.f;
    float totalAmplitude = 0;
    yang::XorshiftRNG rng(m_seed);

    for (int octave = 0; octave < numOctaves; ++octave)
    {
        totalAmplitude += currentAmplitude;
        float localNoise = GetNoise(x, y, maxX, maxY, inputRange, static_cast<uint32_t>(rng()));
        noise += localNoise * currentAmplitude;

        currentAmplitude *= persistance;
        inputRange *= (1.f / persistance);
        //inputRange *= 2.f;
    }

    noise /= totalAmplitude;

    if (noise > m_max) m_max = noise;
    if (noise < m_min) m_min = noise;

    return noise;
}

yang::FVec2 yang::PerlinNoise::GenerateGradient(int x, int y)
{
    //float random = 2920.f * std::sinf(x * 21942.f + y * 171324.f + 8912.f) * std::cosf(x * 23157.f * y * 217832.f + 9758.f);
    //return { std::cosf(random), std::sinf(random) };

    return GenerateGradient(x, y, m_seed);
}

uint32_t yang::PerlinNoise::Get1DRandomNoise(int x)
{
    return Get1DRandomNoise(x, m_seed);
}

uint32_t yang::PerlinNoise::Get2DRandomNoise(int x, int y)
{
    return Get2DRandomNoise(x, y, m_seed);
}

uint32_t yang::PerlinNoise::Get1DRandomNoise(int x, uint32_t seedOverride)
{
    uint32_t kBitNoise1 = 0x68e31da4;
    uint32_t kBitNoise2 = 0xb5297a4d;
    uint32_t kBitNoise3 = 0x1b56c4e9;

    uint32_t mangledBits = static_cast<uint32_t>(x);
    mangledBits *= kBitNoise1;
    mangledBits += seedOverride;
    mangledBits ^= (mangledBits >> 8);
    mangledBits *= kBitNoise2;
    mangledBits ^= (mangledBits << 8);
    mangledBits *= kBitNoise3;
    mangledBits ^= (mangledBits >> 8);

    return mangledBits;
}

uint32_t yang::PerlinNoise::Get2DRandomNoise(int x, int y, uint32_t seedOverride)
{
    int kPrime = 198491317;
    return Get1DRandomNoise(x + (kPrime * y), seedOverride);
}

yang::FVec2 yang::PerlinNoise::GenerateGradient(int x, int y, uint32_t seedOverride)
{
    uint32_t randomNoise = Get2DRandomNoise(x, y, seedOverride);
    float random = (static_cast<float>(randomNoise) / std::numeric_limits<uint32_t>::max()) * 2 * Math::kPi;
    return { std::cosf(random), std::sinf(random) };
}

float yang::PerlinNoise::GetNoise(float x, float y, uint32_t seedOverride)
{
    int x0 = static_cast<int>(std::floor(x));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(std::floor(y));
    int y1 = y0 + 1;

    // generate gradients for the four closest points
    FVec2 topLeftNoise = GenerateGradient(x0, y0, seedOverride);
    FVec2 topRightNoise = GenerateGradient(x1, y0, seedOverride);
    FVec2 bottomLeftNoise = GenerateGradient(x0, y1, seedOverride);
    FVec2 bottomRightNoise = GenerateGradient(x1, y1, seedOverride);

    // Caluculate the delta position (i.e. how far away from the top left grid point is from our point) 
    // and smooth the results.  These will be used as lerp weights.

    float smoothWeightX = Math::SmootherStep(x - x0);
    float smoothWeightY = Math::SmootherStep(y - y0);

    // interpolate the noise, using the smoothed X value as a weight
    float perlinNoiseX1 = GenerateGradient(x0, y0, seedOverride) * FVec2(x - x0, y - y0);
    float perlinNoiseX2 = GenerateGradient(x1, y0, seedOverride) * FVec2(x - x1, y - y0);
    float resultX = Math::Lerp(perlinNoiseX1, perlinNoiseX2, smoothWeightX);

    float perlinNoiseY1 = GenerateGradient(x0, y1, seedOverride) * FVec2(x - x0, y - y1);
    float perlinNoiseY2 = GenerateGradient(x1, y1, seedOverride) * FVec2(x - x1, y - y1);
    float resultY = Math::Lerp(perlinNoiseY1, perlinNoiseY2, smoothWeightX);

    // Finally, interpolate between the resulting X and Y values using the smoothed Y value as a 
    // weight.  This is out final noise value.
    float finalNoise = Math::Lerp(resultX, resultY, smoothWeightY);

    return finalNoise;
}

float yang::PerlinNoise::GetNoise(FVec2 coords, uint32_t seedOverride)
{
    return GetNoise(coords.x, coords.y, seedOverride);
}

float yang::PerlinNoise::GetNoise(int x, int y, int maxX, int maxY, FVec2 inputRange, uint32_t seedOverride)
{
    float noiseGridX = ((static_cast<float>(x) + 0.5f) / maxX) * inputRange.x;
    float noiseGridY = ((static_cast<float>(y) + 0.5f) / maxY) * inputRange.y;
    return GetNoise(noiseGridX, noiseGridY, seedOverride);
}

float yang::PerlinNoise::GetNoise(IVec2 coords, IVec2 maxCoords, FVec2 inputRange, uint32_t seedOverride)
{
    return GetNoise(coords.x, coords.y, maxCoords.x, maxCoords.y, inputRange, seedOverride);
}
