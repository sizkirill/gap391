#include "IComponent.h"
#include <Utils/StringHash.h>

using yang::IComponent;

yang::IComponent::IComponent(Actor* pOwner, const char* name)
    :m_pOwner(pOwner)
    ,m_id(HashName(name))
{
}

IComponent::~IComponent()
{
	
}

yang::Id yang::IComponent::HashName(const char* name)
{
    return StringHash32(name);
    //static std::hash<std::string> hasher;
    //
    //return static_cast<Id>(hasher(name));
}
