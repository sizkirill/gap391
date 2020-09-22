#include "ActorFactory.h"
#include <Application/Resources/Resource.h>
#include <Application/Resources/ResourceCache.h>
#include <Logic/Actor/Actor.h>
#include <Utils/Logger.h>
#include <Utils/TinyXml2/tinyxml2.h>

#include <Utils/StringHash.h>
using yang::ActorFactory;

ActorFactory::ActorFactory()
    :m_nextActorId(0)
{
	
}

ActorFactory::~ActorFactory()
{
	
}

std::shared_ptr<yang::Actor> yang::ActorFactory::CreateActor(const char* filepath)
{
    ResourceCache* pResCache = ResourceCache::Get();
    auto pResource = pResCache->Load<IResource>(filepath);
    return CreateActor(pResource.get());
}

std::shared_ptr<yang::Actor> yang::ActorFactory::CreateActor(IResource* pActorResource)
{
    using namespace tinyxml2;

    XMLDocument doc;
    XMLError error = doc.Parse(reinterpret_cast<const char*>(pActorResource->GetData().data()), pActorResource->GetData().size());
    if (error != tinyxml2::XML_SUCCESS)
    {
        LOG(Error, "Failed to load actor: %s -- %s", pActorResource->GetName().c_str(), XMLDocument::ErrorIDToName(error));
        return nullptr;
    }

    // Loaded the file! Lets grab the node and pass it to an actor
    XMLElement* pRoot = doc.RootElement();
    std::shared_ptr<yang::Actor> pActor = std::make_shared<Actor>(m_nextActorId++);
    if (!pActor->Init(pRoot))
    {
        LOG(Warning, "Failed to init actor");
        return nullptr;
    }

    // Iterating through the children of a node
    using namespace tinyxml2;
    for (XMLElement* pElement = pRoot->FirstChildElement(); pElement != nullptr; pElement = pElement->NextSiblingElement())
    {
        pActor->AddComponent(CreateComponent(pElement, pActor.get()));
    }

    if (!pActor->PostInit())
    {
        LOG(Warning, "Failed to post init actor from file %s", pActorResource->GetName().c_str());
        return nullptr;
    }

    return pActor;
}

void yang::ActorFactory::RegisterComponentCreator(Id id, ComponentFunction pFunction)
{
    m_componentCreatorMap[id] = pFunction;
}

std::unique_ptr<yang::IComponent> yang::ActorFactory::CreateComponent(tinyxml2::XMLElement* pData, Actor* pOwner)
{
    const char* name = pData->Name();
    Id componentId = IComponent::HashName(name);

    // Check if the id is in the map
    auto createItr = m_componentCreatorMap.find(componentId);
    if (createItr == m_componentCreatorMap.end())
    {
        LOG(Error, "No associated creation function for component ID %d", componentId);
        return nullptr;
    }

    std::unique_ptr<IComponent> pComponent = createItr->second(pOwner);

    if (pComponent == nullptr || !pComponent->Init(pData))
    {
        LOG(Error, "Failed to init component (ID: %d)", componentId);
        return nullptr;
    }

    return pComponent;
}
