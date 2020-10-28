#include "Random.h"
#include <random>

XorshiftRNG XorshiftRNG::GlobalRNG = XorshiftRNG();

XorshiftRNG::XorshiftRNG(uint64_t seed)
    :m_state(seed)
{
}

XorshiftRNG::XorshiftRNG()
{
    std::random_device rd;
    m_state = rd();
}

uint64_t XorshiftRNG::operator()()
{
    return GetNext();
}

void XorshiftRNG::Seed(uint64_t seed)
{
    m_state = seed;
}

uint64_t XorshiftRNG::GetNext()
{
    uint64_t x = m_state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    m_state = x;
    return m_state;
}
