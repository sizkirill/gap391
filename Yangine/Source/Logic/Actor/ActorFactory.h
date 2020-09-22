#pragma once
#include <functional>
#include <memory>
#include <Logic/Components/IComponent.h>
#include <Utils/Typedefs.h>
#include <Utils/StringHash.h>

/** \file ActorFactory.h */
/** Factory used to create actors */

//! \namespace yang Contains all Yangine code
namespace yang
{
    class IResource;
/** \class ActorFactory */
/** Factory used to create actors */
class ActorFactory
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    /// Alias for function that create components
    using ComponentFunction = std::function<std::unique_ptr<IComponent>(Actor*)>;

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	ActorFactory();

	/** Default Destructor */
	~ActorFactory();

    /// Create an actor from an XML file. Under the covers calls the overload that takes in IResource*
    /// \param filepath - path to the XML file that contains actor description
    /// \return shared pointer to the new actor. Can be null if factory failed to create the actor
    std::shared_ptr<Actor> CreateActor(const char* filepath);

    /// Create an actor from a raw data resource
    /// \param pActorResource - resource that contains actor description
    /// \return shared pointer to the new actor. Can be null if factory failed to create the actor
    std::shared_ptr<Actor> CreateActor(IResource* pActorResource);

    /// Add component creation function to a lookup table
    /// \param id - component ID to associate the function to
    /// \param pFunction - Creation function to associate \see yang::ActorFactory::ComponentFunction
    void RegisterComponentCreator(Id id, ComponentFunction pFunction);

    template <class Component, class... Args>
    void RegisterComponent(Args... args);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
    
    /// Create component from an XML element and associates it with an actor
    /// \param pData - XML Element, containing the component data
    /// \param pOwner - actor that will own the component
    /// \return unique pointer to the newly created component
    std::unique_ptr<IComponent> CreateComponent(tinyxml2::XMLElement* pData, Actor* pOwner);

    Id m_nextActorId;                                                        ///< ID that next actor will have. Increments after each actor is created
    std::unordered_map<Id, ComponentFunction> m_componentCreatorMap;         ///< Component creation functions lookup table
        
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};

#pragma warning(push)
#pragma warning(disable:4307)

template<class Component, class... Args>
inline void ActorFactory::RegisterComponent(Args... args)
{
    static_assert(std::is_base_of_v<IComponent, Component>, "IComponent should be a base class of Component");
    RegisterComponentCreator(StringHash32(Component::GetName()), 
        [args...](Actor* pOwner)
        {
            return IComponent::CreateComponent<StringHash32(Component::GetName())>(pOwner, args...);
        });
}

#pragma warning(pop)
}