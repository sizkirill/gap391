#pragma once

#include <Logic/Process/IProcess.h>
#include <Logic/Actor/Actor.h>
#include <Utils/Logger.h>
#include <memory>

namespace yang
{
template <class ProcessDescription, class... ComponentTypes>
class BaseProcess : public yang::IProcess
{
    static_assert(std::is_invocable_v<decltype(&ProcessDescription::Update), ProcessDescription, float, ComponentTypes*...>, "Process description doesn't match");
public:
    template<class... DescriptionConstructorArgs>
    BaseProcess(yang::Actor* pOwner, DescriptionConstructorArgs&&... args);

    virtual ~BaseProcess() = default;

    virtual bool Init() override final;

    virtual void Update(float deltaseconds) override final;
protected:
    template<class ComponentType>
    ComponentType* GetComponent();
private:
    std::tuple<ComponentTypes*...> m_components;
    ProcessDescription m_actualProcess;
};

template<class ProcessDescription, class ...ComponentTypes>
template<class ...DescriptionConstructorArgs>
inline BaseProcess<ProcessDescription, ComponentTypes...>::BaseProcess(yang::Actor* pOwner, DescriptionConstructorArgs&& ...args)
    :IProcess(pOwner)
    , m_actualProcess(this, std::forward<DescriptionConstructorArgs>(args)...)
{
}

template<class ProcessDescription, class ...ComponentTypes>
inline bool BaseProcess<ProcessDescription, ComponentTypes...>::Init()
{
    bool success = true;
    auto logErrorIfNull = [&success](auto pComp, const char* compName)
    {
        if (!pComp)
        {
            LOG(Error, "%s was nullptr", compName);
            success = false;
        }
        return pComp;
    };

    m_components = std::make_tuple(logErrorIfNull(m_pOwner->GetComponent<ComponentTypes>(), ComponentTypes::GetName())...);

    return success;
}

template<class ProcessDescription, class ...ComponentTypes>
inline void BaseProcess<ProcessDescription, ComponentTypes...>::Update(float deltaseconds)
{
    std::apply(&ProcessDescription::Update, std::tuple_cat(std::make_tuple(m_actualProcess, deltaseconds), m_components));
}

template<class ProcessDescription, class ...ComponentTypes>
template<class ComponentType>
inline ComponentType* BaseProcess<ProcessDescription, ComponentTypes...>::GetComponent()
{
    return std::get<ComponentType*>(m_components);
}

}