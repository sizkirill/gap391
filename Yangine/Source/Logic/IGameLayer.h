#pragma once
/** \file IGameLayer.h */
/** IGameLayer class description */
#include <Views/IView.h>
#include <Logic/Actor/ActorFactory.h>
#include <Logic/Process/ProcessManager.h>
#include <Logic/Physics/Box2DAdapters.h>
#include <Logic/Scripting/LuaManager.h>
#include <Logic/Collisions/CollisionSystem.h>
#include <memory>
#include <vector>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class ApplicationLayer;
    class IPhysicsSimulation;
    class IContactListener;
    class TiledMap;
/** \class IGameLayer */
/** Interface for the game logic. Must be inherited in the Game project */
class IGameLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	IGameLayer() = default;

	/** Default Destructor */
	virtual ~IGameLayer() = default;

    /// Get the game name for the window title
	virtual const char* GetGameName() const = 0;

    /// Initialize all modules
    /// \param app - application layer
    /// \return true if successful
    virtual bool Init(const ApplicationLayer& app);

    /// Add view to the list of views
    /// \param pView - unique pointer to a View to add
    virtual void AddView(std::unique_ptr<IView> pView);

    /// Update all modules by deltaSeconds
    /// \param deltaSeconds - time passed since last frame
    virtual void Update(float deltaSeconds);

    /// Cleans up memory allocations and 3rd party libraries
    virtual void Cleanup();

    /// Adds process to a process manager
    /// \param pProcess - shared pointer to a process to add
    virtual void AddProcess(std::shared_ptr<IProcess> pProcess);

    // TODO: Add delayed spawning. 
    /// Spawns actor in the world at next frame
    /// \param filepath - path to the XML file that describes the actor
    /// \param whereToSpawn - location where to spawn an actor
    /// \return shared pointer to the spawned actor. Can be null if spawning failed
    virtual std::shared_ptr<Actor> SpawnActor(const char* filepath, std::optional<FVec2> whereToSpawn = {});

    /// Destroys actor at next frame
    /// \param actorId - Id of the actor to destroy
    void DestroyActor(Id actorId);

    /// Notifies actors about collision (places in a collection to handle it later)
    /// \param pActorA - first colliding actor
    /// \param pActorB - second colliding actor
    /// \param isOverlap - true if actors should overlap and not collide
    /// \param state - state of the contact. Does the contact begin or end?
    void NotifyContact(Actor* pActorA, Actor* pActorB, bool isOverlap, ContactState state);

	// TODO: rethink
    /// Registers everything that is needed to be registered to Lua environment.
    /// \param pState - lua_State object to register to
	void RegisterWithLua(lua_State* pState);

    /// Registers this class member function to the Lua environment
    /// \param manager - LuaManager instance to handle the registration
	static void RegisterToLua(const LuaManager& manager);
protected:
    // --------------------------------------------------------------------- //
    // Protected Member Variables
    // --------------------------------------------------------------------- //
    ActorFactory m_actorFactory;                                ///< Instance of ActorFactory that handle actor spawning. \see yang::ActorFactory
    std::unordered_map<Id, std::shared_ptr<Actor>> m_actors;    ///< Hash table of actors, where keys are their ids
    ProcessManager m_processManager;                            ///< Instance of ProcessManager that handles all game processes
    IPhysicsSimulation* m_pPhysics = nullptr;                   ///< Instance of the Physics subsystem. Handles collisions and physics simulation
    IContactListener* m_pContactListener = nullptr;             ///< Contact listener systems that handles collision callbacks

    std::shared_ptr<TiledMap> m_pMap = nullptr;                 ///< Instance of the TiledMap

    /// Alias for a CollisionPair data
    using CollisionPair = std::tuple<Actor*, Actor*, bool, ContactState>;

    std::vector<CollisionPair> m_recentCollisions;              ///< Collection of collisions happened since last frame

	LuaManager m_luaManager;                                    ///< Instance of a LuaManager

    // --------------------------------------------------------------------- //
    // Protected Member Functions
    // --------------------------------------------------------------------- //

    /// Internal helper function. Registers component creator functions
    void RegisterComponents();

    /// Internal helper function to delete a view
    /// \param pView - view to delete
    void DeleteView(IView* pView);
protected: // actually should be private
    std::vector<std::shared_ptr<Actor>> m_actorsToSpawn;        ///< Collection of actors that are going to be spawned at next frame
    std::vector<Id> m_actorsToKill;                             ///< Collection of IDs of actors that are going to be destroyed at next frame
    std::vector<std::unique_ptr<IView>> m_pViews;               ///< Collection of all views
    std::unique_ptr<CollisionSystem> m_pCollisionSystem;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
    void RespondToCollisions();                                 ///< Actually handles collisions and calls appropriate callbacks

    /// Internal helper function. Deletes view by it's index in the vector
    /// \param index - view's index in the vector
    void DeleteView(size_t index);                              
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Get the actor table
    std::unordered_map<Id, std::shared_ptr<Actor>>& GetActors() { return m_actors; }
	/// Get actor by it's ID
    Actor* GetActorById(Id id) const;
	/// Get the instance of lua manager
    const LuaManager* GetLuaManager() const { return &m_luaManager; }
};
}