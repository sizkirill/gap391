#pragma once
/** \file IPhysicsComponent.h */
/** Physics component interface description */
#include <Utils/Math.h>
#include <Utils/Vector2.h>
#include "../IComponent.h"
#include <functional>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class Actor;
/** \class IPhysicsComponent */
/** Base class for physics collision components */

class IPhysicsComponent : public IComponent
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
    /// \param name - Name of the component
	IPhysicsComponent(Actor* pOwner, const char* name);

	/** Default Destructor */
	virtual ~IPhysicsComponent();
    
    /// Execute begin overlap callback
    /// \param pActor - actor that this component's owner starts overlapping
    void HandleBeginOverlap(Actor* pActor);

    /// Execute end overlap callback
    /// \param pActor - actor that this component's owner ends overlapping
    void HandleEndOverlap(Actor* pActor);

    /// Execute begin collision callback
    /// \param pActor - actor that this component's owner starts colliding
    void HandleBeginCollision(Actor* pActor);

    /// Execute end collision callback
    /// \param pActor - actor that this component's owner ends overlapping
    void HandleEndCollision(Actor* pActor);

    /// Set linear velocity of the physics body
    /// \param velocity - Vector2 that represents body's new velocity
    virtual void SetLinearVelocity(FVec2 velocity) = 0;

    /// Apply impulse to physics body center
    /// \param impulse - Vector2 that represents the impulse amount and direction. Amount is in (Kg*m)/s
    virtual void ApplyLinearImpulseToCenter(FVec2 impulse) = 0;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

    /// Alias for callbacks
    using CollisionCallback = std::function<void(Actor* pOther)>;

    CollisionCallback m_onCollisionBegin;   ///< Callback for begin collision
    CollisionCallback m_onCollisionEnd;     ///< Callback for end collision
    CollisionCallback m_onOverlapBegin;     ///< Callback for begin overlap
    CollisionCallback m_onOverlapEnd;       ///< Callback for end overlap

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
    void SetBeginCollision(CollisionCallback handler) { m_onCollisionBegin = handler; }   ///< Set the begin collision callback  
    void SetEndCollision(CollisionCallback handler) { m_onCollisionEnd = handler; }       ///< Set the end collision callback
    void SetBeginOverlap(CollisionCallback handler) { m_onOverlapBegin = handler; }       ///< Set the begin overlap callback
    void SetEndOverlap(CollisionCallback handler) { m_onOverlapEnd = handler; }           ///< Set the end overlap callback
};
}