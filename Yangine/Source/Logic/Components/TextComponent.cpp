#include "TextComponent.h"
#include <cassert>
#include <charconv>
#include <string_view>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <Application/Graphics/Fonts/IFont.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Application/Resources/ResourceCache.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Actor/Actor.h>

using yang::TextComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> yang::IComponent::CreateComponent<StringHash32(TextComponent::GetName())>(Actor* pOwner) { return std::make_unique<TextComponent>(pOwner); };

#pragma warning(pop)

TextComponent::TextComponent(yang::Actor* pOwner)
	:IComponent(pOwner, GetName())
	,m_pTexture(nullptr)
	,m_pFont(nullptr)
	,m_textColor(0xFFFFFFFF)
	,m_pTransform(nullptr)
{
	
}

TextComponent::~TextComponent()
{
	//
}

bool yang::TextComponent::Init(tinyxml2::XMLElement* pData)
{
	const char* pFontPath = pData->Attribute("src");

	if (!pFontPath)
	{
		LOG(Error, "TextComponent must contain path to a font file");
		return false;
	}

	int ptSize = pData->IntAttribute("size");
	if (ptSize == 0)
	{
		LOG(Error, "TextComponent must have font size attribute");
		return false;
	}

	m_pFont = ResourceCache::Get()->Load<IFont>(pFontPath, ptSize);

	if (!m_pFont)
	{
		LOG(Error, "TextComponent failed to get font from ResourceCache");
		return false;
	}

	const char* pText = pData->Attribute("data");
	if (!pText)
	{
		LOG(Warning, "Failed to found string data, text string is not initialized");
		m_pTexture = nullptr;
	}
	else
	{
		m_pTexture = m_pFont->CreateTextureFromString(pText);
	}

	const char* pColor = pData->Attribute("hexColor");
	if (pColor)
	{
		std::string_view colorStr(pColor);
		if (colorStr.length() > 2 && (colorStr.at(1) == 'x' || colorStr.at(1) == 'X'))
		{
			colorStr.remove_prefix(2);
		}
		ui32 color;
		std::from_chars(colorStr.data(), colorStr.data() + colorStr.length(), color, 16);
		m_textColor = color;
	}

	return true;
}

bool yang::TextComponent::PostInit()
{
	m_pTransform = GetOwner()->GetComponent<TransformComponent>();
	if (!m_pTransform)
	{
		LOG(Error, "TextComponent requires TransformComponent");
		return false;
	}
	return true;
}

bool yang::TextComponent::Render(IGraphics* pGraphics)
{
	assert(m_pTransform);

	const FVec2& position = m_pTransform->GetPosition();
	pGraphics->DrawTexture(m_pTexture.get(), IVec2(position));

	return false;
}

void yang::TextComponent::UpdateText(const std::string& text)
{
	m_pTexture = m_pFont->CreateTextureFromString(text.c_str());
	m_pTexture->SetTint(m_textColor);
}

yang::IVec2 yang::TextComponent::GetTextureDimensions() const
{
	return m_pTexture ? m_pTexture->GetDimensions() : IVec2(0, 0);
}

void yang::TextComponent::SetColor(IColor color)
{
	m_textColor = color;
	if (m_pTexture)
		m_pTexture->SetTint(color);
}
