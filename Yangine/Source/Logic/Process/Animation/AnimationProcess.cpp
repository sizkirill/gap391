#include "AnimationProcess.h"
#include <cassert>
#include <Utils/Logger.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Application/Resources/ResourceCache.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Components/SpriteComponent.h>
#include <Logic/Components/Animation/AnimationComponent.h>

using yang::AnimationProcess;

AnimationProcess::AnimationProcess(yang::Actor* pOwner)
    :IProcess(pOwner)
    ,m_pAnimationData(nullptr)
    ,m_pSpriteData(nullptr)
    ,m_currentFrameTime(0)
{
	
}

AnimationProcess::~AnimationProcess()
{
	
}

void yang::AnimationProcess::Update(float deltaSeconds)
{
    m_currentFrameTime -= deltaSeconds;
    if (m_currentFrameTime <= 0)
    {
        auto pAnimationSequence = m_pAnimationData->GetActiveSequence();
        assert(pAnimationSequence);
        ++pAnimationSequence->m_currentFrameIndex;

        if (pAnimationSequence->m_currentFrameIndex >= pAnimationSequence->m_frameCount)
        {
            if (pAnimationSequence->m_isLooping)
            {
                pAnimationSequence->m_currentFrameIndex = 0;
            }
            else
            {
                // This animation sequence is done, pause the process. When the animation sequence changes - resume it (TODO: think how?)
                Pause();
                return;
            }
        }

        auto& currentFrame = pAnimationSequence->m_frameData[pAnimationSequence->m_currentFrameIndex];
        m_currentFrameTime = currentFrame.m_duration;

        m_pSpriteData->SetSourceRect(currentFrame.m_frameRect);
        const auto& currentTexturePath = m_pAnimationData->GetCurrentTexturePath();

        // Probably this is faster than comparing paths of two textures to determine if they're the same. If the resource is already loaded, it is O(1) access
        auto pAnimationTexture = ResourceCache::Get()->Load<ITexture>(currentTexturePath.c_str());
        // So, should we check if the textures are the same or just assigning is fine? it is shared ptr, so it's not free.
        m_pSpriteData->SetTexture(pAnimationTexture);
    }
}

bool yang::AnimationProcess::Init()
{
    m_pAnimationData = m_pOwner->GetComponent<AnimationComponent>();
    if (!m_pAnimationData)
    {
        LOG(Error, "Animation process requires actor to have animation component");
        return false;
    }

    m_pSpriteData = m_pOwner->GetComponent<SpriteComponent>();
    if (!m_pSpriteData)
    {
        LOG(Error, "Animation process requires actor to have sprite component");
        return false;
    }

    return true;
}
