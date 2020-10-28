#include "RuleContainer.h"

void RuleContainer::AddRule(char var, float probability, std::string&& rule)
{
    assert(probability <= 1.f);
    m_rules.emplace(var, std::make_pair(probability, std::move(rule)));
}

void RuleContainer::AddConstant(char constant)
{
    assert(m_rules.find(constant) == m_rules.end());
    m_rules.emplace(constant, std::make_pair(1.f, std::string(1, constant)));
}

void RuleContainer::Finalize()
{
    float totalProbability = 0;
    char currentKey = 0;
    for (auto it = m_rules.begin(); it != m_rules.end(); ++it)
    {
        auto& [key, valuePair] = *it;
        auto& [probability, rule] = valuePair;

        if (key == currentKey)
        {
            totalProbability += probability;
        }
        else
        {
            if (currentKey != 0)
            {
                assert(totalProbability <= 1.f);

                if (totalProbability < 1.f)
                {
                    AddRule(currentKey, 1.f - totalProbability, std::string(1, currentKey));
                }
            }

            currentKey = key;
            totalProbability = probability;
        }
    }
}
