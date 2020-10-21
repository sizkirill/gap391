#include "RuleContainer.h"

void RuleContainer::AddRule(char var, float probability, std::string&& rule)
{
#ifdef DEBUG
    if (m_containerStatus != Status::kInProcess)
    {
        LOG(Error, "RuleContainer status was %d. Was trying to emplace \"%c\" - %f - %s", static_cast<int>(m_containerStatus), var, probability, rule.data());
        m_containerStatus = Status::kInvalid;
        return;
    }
#endif

    assert(probability <= 1.f);
    m_rules.emplace(var, std::make_pair(probability, std::move(rule)));

#ifdef DEBUG
    auto [startIt, endIt] = m_rules.equal_range(var);
    float probSum = 0;
    for (auto it = startIt; it != endIt; ++it)
    {
        auto& [key, probPair] = *it;
        auto& [probability, ruleResult] = probPair;
        probSum += probability;
    }
    assert(probSum <= 1.0f);
#endif
}

void RuleContainer::AddConstant(char constant)
{
#ifdef DEBUG
    if (m_containerStatus != Status::kInProcess)
    {
        LOG(Error, "RuleContainer status was %d. Was trying to emplace constant \"%c\"", static_cast<int>(m_containerStatus), constant);
        m_containerStatus = Status::kInvalid;
        return;
    }
#endif

    assert(m_rules.find(constant) == m_rules.end());
    m_rules.emplace(constant, std::make_pair(1.f, std::string(1, constant)));
}

void RuleContainer::Finalize()
{
#ifdef DEBUG
    if (m_containerStatus != Status::kInProcess)
    {
        LOG(Error, "RuleContainer status was %d. Was trying to finalize an invalid or already finalized container", static_cast<int>(m_containerStatus));
        m_containerStatus = Status::kInvalid;
        return;
    }
#endif

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

#ifdef DEBUG
    m_containerStatus = Status::kFinal;
#endif
}
