#include <cassert>
#include "AnimationComponent.h"
#include <Logic/Scripting/LuaManager.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <Utils/StringHash.h>

using yang::AnimationComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> IComponent::CreateComponent<StringHash32(AnimationComponent::GetName())>(yang::Actor* pOwner)
{
    return std::make_unique<AnimationComponent>(pOwner);
};

#pragma warning(pop)

AnimationComponent::AnimationComponent(yang::Actor* pOwner)
    :IComponent(pOwner, GetName())
    ,m_pActiveSequence(nullptr)
    ,m_sequenceCount(0)
{
	
}

AnimationComponent::~AnimationComponent()
{
	
}

bool yang::AnimationComponent::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    m_sequenceCount = pData->IntAttribute("sequenceCount");
    if (m_sequenceCount == 0)
    {
        LOG(Error, "Animation component requires sequence count to be specified");
        return false;
    }

    // Reserve the space for sequences
    m_sequences.reserve(m_sequenceCount);

    const char* pDefaultSrcPath = pData->Attribute("defaultSrc");
    if (!pDefaultSrcPath)
    {
        LOG(Error, "Animation component requires default src path to a texture");
        return false;
    }
    m_defaultTexturePath = pDefaultSrcPath;

    m_defaultFrameRate = pData->FloatAttribute("defaultFrameRate");
    if (m_defaultFrameRate == 0)
    {
        LOG(Error, "Animation component requires to initialize default framerate");
        return false;
    }

    const char* pActiveSequence = pData->Attribute("startingSequence");
    if (!pActiveSequence)
    {
        LOG(Error, "Starting sequence should be explicitly specified");
        return false;
    }

    for (XMLElement* pSequence = pData->FirstChildElement("AnimationSequence"); pSequence != nullptr; pSequence = pSequence->NextSiblingElement("AnimationSequence"))
    {
        AnimationSequence sequence;
        sequence.m_frameCount = pSequence->IntAttribute("frameCount");

        if (sequence.m_frameCount == 0)
        {
            LOG(Error, "Animation sequence requires number of frames");
            return false;
        }

        // reserve space for frames
        sequence.m_frameData.reserve(sequence.m_frameCount);

        const char* pSeqName = pSequence->Attribute("name");
        if (!pSeqName)
        {
            LOG(Error, "Animation sequence requires a name");
            return false;
        }
        sequence.m_name = pSeqName;

        const char* pIsLooping = pSequence->Attribute("isLooping");
        if (pIsLooping && std::string_view(pIsLooping) == "true")
        {
            sequence.m_isLooping = true;
        }
        else
        {
            sequence.m_isLooping = false;
        }

        const char* pTextureSrc = pSequence->Attribute("src");
        if (pTextureSrc && std::string_view(pTextureSrc) != m_defaultTexturePath)
        {
            sequence.m_texturePath = pTextureSrc;
        }
        else
        {
            sequence.m_texturePath = {};
        }

        float framerate = pSequence->FloatAttribute("framerate");
        if (framerate == 0)
        {
            sequence.m_framerate = m_defaultFrameRate;
        }
        else
        {
            sequence.m_framerate = framerate;
        }

        for (XMLElement* pFrame = pSequence->FirstChildElement("Frame"); pFrame != nullptr; pFrame = pFrame->NextSiblingElement("Frame"))
        {
            Frame frame;
            IRect frameRect;

            frameRect.x = pFrame->IntAttribute("x", -1);
            if (frameRect.x == -1)
            {
                LOG(Error, "Frame rect X coordinate should be initialized");
                return false;
            }
            frameRect.y = pFrame->IntAttribute("y", -1);
            if (frameRect.y == -1)
            {
                LOG(Error, "Frame rect Y coordinate should be initialized");
                return false;
            }
            frameRect.width = pFrame->IntAttribute("w", -1);
            if (frameRect.width == -1)
            {
                LOG(Error, "Frame rect width coordinate should be initialized");
                return false;
            }
            frameRect.height = pFrame->IntAttribute("h", -1);
            if (frameRect.height == -1)
            {
                LOG(Error, "Frame rect height coordinate should be initialized");
                return false;
            }
            frame.m_frameRect = frameRect;

            const char* pTextureSrc = pFrame->Attribute("src");
            if (pTextureSrc)
            {
                bool isNotSameAsDefault = std::string_view(pTextureSrc) != m_defaultTexturePath;
                bool isNotSameAsSequenceDefault = sequence.m_texturePath && std::string_view(pTextureSrc) != *sequence.m_texturePath;
                if (isNotSameAsDefault && isNotSameAsSequenceDefault)
                {
                    frame.m_texturePath = pTextureSrc;
                }
                else
                {
                    frame.m_texturePath = {};
                }
            }

            float duration = pFrame->FloatAttribute("duration");
            if (duration == 0)
            {
                frame.m_duration = 1.f / sequence.m_framerate;
            }
            else
            {
                frame.m_duration = duration;
            }

            sequence.m_frameData.push_back(std::move(frame));
        }

        m_sequences.emplace(pSeqName, std::move(sequence));
    }

    auto currentSeqIt = m_sequences.find(pActiveSequence);
    if (currentSeqIt == m_sequences.cend())
    {
        LOG(Error, "Cannot find the specified starting sequence");
        return false;
    }
    m_pActiveSequence = &(currentSeqIt->second);

    return true;
}

void yang::AnimationComponent::RegisterToLua(const LuaManager& manager)
{
    manager.ExposeToLua("GetActiveAnimationSequence", &AnimationComponent::GetActiveSequence);
    manager.ExposeToLua("SetActiveAnimationSequence", &AnimationComponent::SetActiveSequence);
}

yang::AnimationComponent::AnimationSequence* yang::AnimationComponent::GetActiveSequence() const
{
    return m_pActiveSequence;
}

void yang::AnimationComponent::SetActiveSequence(const std::string& name)
{
    auto it = m_sequences.find(name);
    if (it != m_sequences.cend())
    {
        m_pActiveSequence = &(it->second);
    }
}

const std::string& yang::AnimationComponent::GetCurrentTexturePath() const
{
    assert(m_pActiveSequence);
    auto& frameTexturePath = m_pActiveSequence->m_frameData[m_pActiveSequence->m_currentFrameIndex].m_texturePath;
    auto& sequenceTexturePath = m_pActiveSequence->m_texturePath;
    if (frameTexturePath.has_value())
    {
        return *frameTexturePath;
    }
    else if (sequenceTexturePath.has_value())
    {
        return *sequenceTexturePath;
    }
    else
    {
        return m_defaultTexturePath;
    }

}
