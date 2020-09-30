#pragma once
/** \file AnimationProcess.h */
/** Animation process description */

#include <Logic/Process/BaseProcess.h>
#include <Logic/Components/Animation/AnimationComponent.h>
#include <Logic/Components/SpriteComponent.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
	class AnimationComponent;
	class SpriteComponent;

	/** \class AnimationProcessDescription */
	/** Handles the animation of an actor that has SpriteComponent & AnimationComponent. */
class AnimationProcessDescription
{
public:
	AnimationProcessDescription(IProcess* pOwner);
	void Update(float deltaSeconds, yang::AnimationComponent* pAnimationData, SpriteComponent* pSpriteData);
private:
	float m_currentFrameTime = 0;
	IProcess* m_pOwner = nullptr;
};

/// <summary>
///  Alias for AnimationProcess
/// </summary>
using AnimationProcess = yang::BaseProcess<AnimationProcessDescription, AnimationComponent, SpriteComponent>;
}