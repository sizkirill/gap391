#include "AnimationProcess.h"
#include <cassert>
#include <Utils/Logger.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Application/Resources/ResourceCache.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Components/SpriteComponent.h>
#include <Logic/Components/Animation/AnimationComponent.h>

yang::AnimationProcessDescription::AnimationProcessDescription(IProcess* pOwner)
    :m_pOwner(pOwner)
{
}

void yang::AnimationProcessDescription::Update(float deltaSeconds, yang::AnimationComponent* pAnimationData, SpriteComponent* pSpriteData)
{
    m_currentFrameTime -= deltaSeconds;
    if (m_currentFrameTime <= 0)
    {
        auto pAnimationSequence = pAnimationData->GetActiveSequence();
        assert(pAnimationSequence);
        ++(pAnimationSequence->m_currentFrameIndex);

        if (pAnimationSequence->m_currentFrameIndex >= pAnimationSequence->m_frameCount)
        {
            if (pAnimationSequence->m_isLooping)
            {
                pAnimationSequence->m_currentFrameIndex = 0;
            }
            else
            {
                // This animation sequence is done, pause the process. When the animation sequence changes - resume it (TODO: think how?)
                m_pOwner->Pause();
                return;
            }
        }

        auto& currentFrame = pAnimationSequence->m_frameData[pAnimationSequence->m_currentFrameIndex];
        m_currentFrameTime = currentFrame.m_duration;

        pSpriteData->SetSprite(currentFrame.m_pSprite);
    }
}
