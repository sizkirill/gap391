#pragma once
/** \file IPhysicsSimulation.h */
/** Physics Simulation interface description */

#include <Utils/Typedefs.h>
#include <Utils/Math.h>
#include <Utils/Vector2.h>
#include <memory>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class IContactListener;
    class IPhysicsComponent;
    class Actor;
/** \class IPhysicsSimulation */
/** Base interface for physics engine abstraction */
class IPhysicsSimulation
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	IPhysicsSimulation();

	/** Default Destructor */
	virtual ~IPhysicsSimulation();

    /// Initialize the physics simulation
    /// \return true if successful
    virtual bool Init() = 0;

    /// Update the simulation
    /// \param deltaSeconds - amount of seconds since last frame
    virtual void Update(float deltaSeconds) = 0;

    /// Set the contact listener to use by physics simulation
    /// \param pListener - \see yang::IContactListener
    virtual void SetContactListener(IContactListener* pListener) = 0;

    // TODO: Same with DynamicBody

    /// Create the Physics system
    /// \param gravity - gravity vector to use by physics system (in pixels/sec^2 , will be converted to the appropriate units inside)
    /// \return unique pointer to the physics system
    static std::unique_ptr<IPhysicsSimulation> Create(FVec2 gravity);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}