#pragma once
#include <Utils/Random.h>
#include <Utils/Vector2.h>

namespace yang
{
class PerlinNoise
{
public:
    PerlinNoise(uint32_t seed = 0x1b56c4e9);

    float GetNoise(float x, float y);
    float GetNoise(FVec2 coords);

    float GetNoise(int x, int y, int maxX, int maxY, FVec2 inputRange);
    float GetNoise(IVec2 coords, IVec2 maxCoords, FVec2 inputRange);

    float GetMin() const { return m_min; }
    float GetMax() const { return m_max; }

    auto NormalizationFunc() const {
        return [this](float val)
        {
            return (val - m_min) / (m_max - m_min);
            //return std::fabs(val) / std::max(std::fabs(m_min), m_max);

            //return (val + 1.f) / 2.f;
        };
    }

    float NormalizedNoise(int x, int y, int maxX, int maxY, FVec2 inputRange, int numOctaves, float persistance);
private:
    uint32_t m_seed;
    XorshiftRNG m_rng;
    float m_min;
    float m_max;

    FVec2 GenerateGradient(int x, int y);
    uint32_t Get1DRandomNoise(int x);
    uint32_t Get2DRandomNoise(int x, int y);

    static uint32_t Get1DRandomNoise(int x, uint32_t seedOverride);
    static uint32_t Get2DRandomNoise(int x, int y, uint32_t seedOverride);
    static FVec2 GenerateGradient(int x, int y, uint32_t seedOverride);
    static float GetNoise(float x, float y, uint32_t seedOverride);
    static float GetNoise(FVec2 coords, uint32_t seedOverride);
    static float GetNoise(int x, int y, int maxX, int maxY, FVec2 inputRange, uint32_t seedOverride);
    static float GetNoise(IVec2 coords, IVec2 maxCoords, FVec2 inputRange, uint32_t seedOverride);
};
}