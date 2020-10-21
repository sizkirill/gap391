#include "Random.h"
#include <random>
#include <Logic/Scripting/LuaManager.h>

yang::XorshiftRNG yang::XorshiftRNG::GlobalRNG = yang::XorshiftRNG();

yang::XorshiftRNG::XorshiftRNG(uint64_t seed)
    :m_state(seed)
{
}

yang::XorshiftRNG::XorshiftRNG()
{
    std::random_device rd;
    m_state = rd();
}

uint64_t yang::XorshiftRNG::operator()()
{
    return GetNext();
}

void yang::XorshiftRNG::Seed(uint64_t seed)
{
    m_state = seed;
}

void yang::XorshiftRNG::ExposeToLua(const LuaManager& luaenv)
{
    luaenv.ExposeToLua<float(XorshiftRNG::*)()>("FRand", &XorshiftRNG::FRand<float>);
    luaenv.ExposeToLua<int(XorshiftRNG::*)(int, int)>("Rand", &XorshiftRNG::Rand<int>);
}

uint64_t yang::XorshiftRNG::GetNext()
{
    uint64_t x = m_state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    m_state = x;
    return m_state;
}
