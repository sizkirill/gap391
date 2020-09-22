#pragma once
/** \file AnimationProcess.h */
/** Animation process description */

#include "..\IProcess.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
    class AnimationComponent;
    class SpriteComponent;

/** \class AnimationProcess */
/** Handles the animation of an actor that has SpriteComponent & AnimationComponent. */
class AnimationProcess
	: public IProcess
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	
    /// Constructor
    /// \param pOwner - actor that owns this process
	AnimationProcess(Actor* pOwner);

	/** Default Destructor */
	~AnimationProcess();

    /// Update the process by deltaSeconds
    /// Effectively makes appropriate changes to SpriteComponent, depending on the animation properties
    /// \param deltaSeconds - time since last frame
    virtual void Update(float deltaSeconds) override final;

    /// Initializes the process
    /// \return true if successfully initialized
    virtual bool Init() override final;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

    // TODO: Think if it is safe to cache these pointers here. Maybe they can change during runtime and in best case we will update non-relevant components. And in worst case - the pointers will be invalidated
    // For now, let's consider that actor's components are not changing their location in memory, they just exist.
    AnimationComponent* m_pAnimationData;       ///< Owner actor's animation component
    SpriteComponent* m_pSpriteData;             ///< Owner actor's sprite component
    float m_currentFrameTime;                   ///< Time spent in the current frame

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}