#pragma once
#include "TransformationRule.h"
#include <vector>

class RuleContainer
{
public:
    RuleContainer(std::string_view pathToXml);

    template <class RandomDevice>
    TransformationRule& GetRandomRule(RandomDevice&& rd);

    template <class RandomDevice>
    TransformationRule& GetWeightedRandomRule(RandomDevice&& rd);
private:
    std::vector<TransformationRule> m_ruleContainer;
};

template<class RandomDevice>
inline TransformationRule& RuleContainer::GetRandomRule(RandomDevice&& rd)
{
    static_assert(std::is_integral_v<decltype(std::declval<RandomDevice>()())>, "RandomDevice should produce Integer values");
    
    return m_ruleContainer[rd() % m_ruleContainer.size()];
}

template<class RandomDevice>
inline TransformationRule& RuleContainer::GetWeightedRandomRule(RandomDevice&& rd)
{
    static_assert(std::is_integral_v<decltype(std::declval<RandomDevice>()())>, "RandomDevice should produce Integer values");

    auto rngResult = rd() % 100;
    int accumulator = 0;

    for (auto& rule : m_ruleContainer)
    {
        accumulator() += rule.GetWeight();
        if (rngResult < accumulator)
        {
            if (rule.ShouldBeUsedOnce())
            {
                rule.ResetWeight();
            }

            return rule;
        }
    }
}
