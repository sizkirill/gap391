#pragma once
/** \file Box2DPhysics.h */
/** Box2D physics simulation description */

#include ".\IPhysicsSimulation.h"
#include <Logic/Physics/Box2DAdapters.h>
// TODO: think about it
#include <Box2D/Box2D.h>

// TODO: kPixelsPerMeter
// TODO: Accumulator (Fixed Time Step)
// MAYBE: https://www.iforce2d.net/b2tut/

/// \class PhysicsDebug
/// Draws shapes that represent physics bodies for debug purposes
class PhysicsDebug : public b2Draw
{
    // Only override the functions that you'll be using
    // DrawRect, etc..
};

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class Box2DPhysics */
/** Physics Simulation based on the Box2D physics engine */
class Box2DPhysics
	: public IPhysicsSimulation
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
   
    /// Constructor
    /// \param gravity - gravity vector to use by Box2D engine (in pixels/sec^2 , will be converted to the appropriate units inside)
	Box2DPhysics(FVec2 gravity);

	/** Default Destructor */
	~Box2DPhysics();

    /// Initialize Box2D physics
    /// \return true if successful
    virtual bool Init() override final;

    /// Update Box2D physics simulation
    /// \param deltaSeconds - amount of seconds since last frame
    virtual void Update(float deltaSeconds) override final;

    /// Set the contact listener to use by physics simulation.
    /// \param pListener - \see yang::IContactListener
    virtual void SetContactListener(IContactListener* pListener) override final;

    /// Convert yang::FVec to b2Vec2. Duplicates the same function in Box2DAdapters.h. TODO: remove/deprecate
    static b2Vec2 ConvertFVec(FVec2 vec);
    /// Convert b2Vec2 to yang::FVec2. Duplicates the same function in Box2DAdapters.h. TODO: remove/deprecate
    static FVec2 ConvertB2Vec(b2Vec2 vec);
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    b2World m_world;        ///< Instance of the Box2D physics world
    float m_accumulator;    ///< Stores amount of time that has passed since last simulation update. (We don't neccessarily update the simulation each frame)

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Get the Box2D world
    b2World* GetWorld() { return &m_world; }
};
}