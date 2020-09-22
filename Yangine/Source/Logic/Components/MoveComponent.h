#pragma once
/** \file MoveComponent.h */
/** MoveComponent description */

#include ".\IComponent.h"
#include <Utils/Math.h>
#include <Utils/Vector2.h>
#include <Utils/Typedefs.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
	class LuaManager;
    class TransformComponent;
    class Actor;
/** \class MoveComponent */
/** Actor component that stores data for the actor movement */
class MoveComponent
	: public IComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param pOwner - actor, that owns this component
	MoveComponent(Actor* pOwner);

	/** Default Destructor */
	~MoveComponent();

    /// Initialize the component from an XML element
    /// \param pData - XML Element that contains component data
    /// \return true if initialized successfully
    virtual bool Init(tinyxml2::XMLElement* pData) override final;

    // TODO: make this not a virtual function, so it will be called only where it should be called. Right now it is unusable.
    /// Updates this component by deltaSeconds time
    /// \param deltaSeconds - number of seconds since last frame
    //virtual void Update(float deltaSeconds) override final;

    /// Name of this component, used for hashing
    /// \return "MoveComponent"
    static constexpr const char* GetName() { return "MoveComponent"; }

    /// Registers member functions to Lua environment
    /// Not intended for use outside of IGameLayer::Init
    /// \param manager - the Lua environment manager \see yang::LuaManager
	static void RegisterToLua(const LuaManager& manager);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

    float m_maxSpeed;

    FVec2 m_velocity;
    FVec2 m_acceleration;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Get the velocity vector
    FVec2 GetVelocity() const { return m_velocity; }

    /// Set the new velocity.
    /// \param velocity - new velocity
    void SetVelocity(FVec2 velocity);

    /// Get the movement speed
	float GetSpeed() const { return m_velocity.Length(); }

    /// Set the movement speed
    /// \param speed - speed in pixels/sec
    void SetSpeed(float speed);

    /// Get the acceleration vector
    FVec2 GetAcceleration() const { return m_acceleration; }

    /// Set the acceleration value
    /// \param acceleration - new acceleration vector
    void SetAcceleration(FVec2 acceleration) { m_acceleration = acceleration; }

    /// Rotates the velocity vector clockwise
    /// \param angle - angle in radians to rotate
	void RotateVelocity(float angle);

    /// Rotates the acceleration vector clockwise
    /// \param angle - angle in radians to rotate
    void RotateAcceleration(float angle);

};
}