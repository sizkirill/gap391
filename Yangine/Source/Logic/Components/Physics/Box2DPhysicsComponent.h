#pragma once
/** \file Box2DPhysicsComponent.h */
/** Box2D physics component description */

#include ".\IPhysicsComponent.h"

class b2World;
class b2Body;
class b2Fixture;

//! \namespace yang Contains all Yangine code
namespace yang
{
    class TransformComponent;
    class IPhysicsSimulation;
/** \class Box2DPhysicsComponent */
/** Physics component for Box2D physics engine */
class Box2DPhysicsComponent
	: public IPhysicsComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param pOwner - Actor that owns the component
    /// \param pPhysics - Physics simulation used by this component. (Assumes that it is box2D simulation)
	Box2DPhysicsComponent(Actor* pOwner, IPhysicsSimulation* pPhysics);
    
    /// The name of this component
    /// \return "Box2DPhysicsComponent"
    static constexpr const char* GetName() { return "Box2DPhysicsComponent"; }

    /// Initialize the component from an XML element
    /// \param pData - XML Element that contains component data
    /// \return true if initialized successfully
    virtual bool Init(tinyxml2::XMLElement* pData) override final;

    /// Obtains reference to actor's transform component
    /// \return true if actor has transform component
    virtual bool PostInit() override final;

    /// Update physics data by delta time
    /// \param deltaSeconds - amount of seconds since last frame
    virtual void Update(float deltaSeconds) override final;

    /// Set linear velocity of the physics body
    /// \param velocity - Vector2 that represents body's new velocity
    virtual void SetLinearVelocity(FVec2 velocity) override final;

    /// Apply impulse to physics body center
    /// \param impulse - Vector2 that represents the impulse amount and direction. Amount is in (Kg*m)/s
    virtual void ApplyLinearImpulseToCenter(FVec2 impulse) override final;

	/** Default Destructor */
	~Box2DPhysicsComponent();


private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

    b2World* m_pWorld;                  ///< Box2D world
    b2Body* m_pBody;                    ///< Box2D body of the object

    // TODO: make this a vector
    b2Fixture* m_pFixture;              ///< Box2D fixture attached to a body.

    TransformComponent* m_pTransform;   ///< Owner actor's transform component

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

};
}