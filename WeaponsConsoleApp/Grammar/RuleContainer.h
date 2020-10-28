#pragma once
#include <unordered_map>
#include <string>
#include <cassert>
#include <map>

class RuleContainer
{
public:
    RuleContainer() = default;
    void AddRule(char var, float probability, std::string&& rule);
    void AddConstant(char constant);
    void Finalize();

    template <class RandomDevice>
    std::string GetRuleResult(char var, RandomDevice&& rd) const;
private:
    std::multimap<char, std::pair<float, std::string>> m_rules;
};

template<class RandomDevice>
inline std::string RuleContainer::GetRuleResult(char var, RandomDevice&& rd) const
{
    static_assert(std::is_same_v<decltype(std::declval<RandomDevice>()()), float>, "RandomDevice should produce floating point values");

    auto [startIt, endIt] = m_rules.equal_range(var);
    float totalProbability = 0;

    float choice = rd();
    assert(choice <= 1.f);

    for (auto it = startIt; it != endIt; ++it)
    {
        auto& [key, valuePair] = *it;
        auto& [probability, rule] = valuePair;
        totalProbability += probability;

        if (choice < totalProbability)
            return rule;
    }
    return std::string(1, var);
}
