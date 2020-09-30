#pragma once
#include <memory>
#include <unordered_map>
#include <Logic/Components/IComponent.h>
#include <Utils/Typedefs.h>
#include <Utils/Matrix.h>
#include <Utils/Logger.h>
#include <Utils/StringHash.h>
/** \file Actor.h */
/** Actor class description */

namespace tinyxml2
{
    class XMLElement;
}

//! \namespace yang Contains all Yangine code
namespace yang
{
	class LuaManager;
    class IView;

/** \class Actor */
/** Represents an object in the game */
class Actor
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/// Constructor
    /// \param id - actor unique id
	Actor(Id id);

	/** Default Destructor */
	~Actor();

    /// Inits actor from XML node
    /// \param pData - pointer to XMLElement (XML node) to init from
    /// \return bool - true if initialization was successful, otherwise false
    bool Init(tinyxml2::XMLElement* pData);

    /// PostInits the actor. (Literally calls PostInit on every component that the actor has)
    /// \return bool - true if PostInit was successfull
    bool PostInit();

    /// Destroys the actor. (Queues the DestroyActor event)
    void Destroy();

    /// Updates the actor (calls update on each component that the actor has)
    /// \param deltaSeconds - time since last frame
    void Update(float deltaSeconds);

    /// Renders the actor (calls Render on each component)
    /// \param pGraphics - pointer to renderer to use
    void Render(IGraphics* pGraphics); // TODO: const correctness
    
    /// Adds the component to an actor
    /// \param pComponent - unique ptr to a component to add
    void AddComponent(std::unique_ptr<IComponent> pComponent);

    /// Templated version of AddComponent. Can be used to add components to an actor without loading from XML
    /// Note: currently not supported
    /// \tparam ComponentType - ComponentType to add
    /// \param args - arguments to construct the Component
    /// \return bool - true if component was successfully added, false if actor already has such component
    template <class ComponentType, class... Args>
    [[deprecated]] bool AddComponent(Args&&... args);

    /// Gets the component with specified id
    /// \param id - ID of the component you want to get
    /// \return IComponent* - pointer to an abstract component or nullptr if actor doesn't have such component
    IComponent* GetComponent(Id id) const;

    /// Gets the component of a specified type
    /// \tparam ComponentType - type of the component to get. Must be a child of IComponent
    /// \return pointer to a component of specified type or nullptr if actor doesn't have a component of that type.
    template <class ComponentType>
    ComponentType* GetComponent() const;

	/// Gets the component with specified name
	/// Implementation mostly for lua exposure, prefer other overloads
	/// \param name - component name
	/// \return IComponent* - pointer to an abstract component or nullptr if actor doesn't have such component
	IComponent* GetComponent(const char* name) const;

    /// Gets this actor's id
    /// \return Id - ID of this actor
    Id GetId() { return m_id; }

	/// Registers member functions to Lua environment
	/// Not intended for use outside of IGameLayer::Init
    /// \param manager - the Lua environment manager \see yang::LuaManager
	static void RegisterToLua(const LuaManager& manager);
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    Id m_id;                                                            ///< Actor unique ID
    std::unordered_map<Id, std::unique_ptr<IComponent>> m_components;   ///< Lookup table for components by component Id.   
    IView* m_pOwningView;                                               ///< View that owns the actor. Can be null
    std::string m_tag;                                                  ///< Actor name
    uint32_t m_hashTag;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    template <uint32_t ComponentHashName>
    IComponent* GetComponent() const;

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Set the owning view
    /// \param pView - actor's owning view
    void SetOwningView(IView* pView) { m_pOwningView = pView; }

    /// Get the owning view
    /// \return actor's owning view. Can be null
    IView* GetOwningView() const { return m_pOwningView; }
    
    /// Get actor's tag
    /// \return actor's tag
    const std::string& GetTag() const { return m_tag; }

    /// Get actor's hashed tag
    /// \return actor's hashed tag
    uint32_t GetHashTag() const { return m_hashTag; }
};

// this actually doesn't work right now.
// TODO(ksizykh): right now when calling this function, the component is not getting initialized.
template<class ComponentType, class ...Args>
inline bool Actor::AddComponent(Args&& ...args)
{
    static_assert(std::is_base_of_v<IComponent, ComponentType>, "ComponentType is not child of IComponent");
    std::unique_ptr<IComponent> pComponent = std::make_unique<ComponentType>(std::forward<Args...>(args...));
    if (m_components[pComponent->GetId()])
    {
        LOG(Error, "Actor already has that type of component");
        return false;
    }
    m_components[pComponent->GetId()] = std::move(pComponent);
    return true;
}

template<class ComponentType>
inline ComponentType* Actor::GetComponent() const
{
    static_assert(std::is_base_of_v<IComponent, ComponentType>, "ComponentType is not child of IComponent");
    return static_cast<ComponentType*>(GetComponent<StringHash32(ComponentType::GetName())>());
}

template<uint32_t ComponentHashName>
inline IComponent* Actor::GetComponent() const
{
    auto itr = m_components.find(ComponentHashName);
    if (itr == m_components.end())
    {
        LOG(Warning, "Actor (ID: %d) doesn't have that component (ID: %d)", m_id, ComponentHashName);
        return nullptr;
    }
    return itr->second.get();
}
}
