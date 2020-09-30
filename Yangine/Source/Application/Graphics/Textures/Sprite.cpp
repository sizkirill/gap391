#include "Sprite.h"
#include <Application/Resources/ResourceCache.h>
#include <Utils/Logger.h>
#include <Utils/TinyXml2/tinyxml2.h>

yang::Sprite::Sprite(std::shared_ptr<ITexture> pTexture, IRect sourceRect, TextureDrawParams params)
    :m_pSourceTexture(pTexture)
    ,m_textureSourceRect(sourceRect)
    ,m_drawParams(params)
{
}

bool yang::Sprite::Init(std::string_view pathToTexture, IRect sourceRect, TextureDrawParams params)
{
    if (auto pResourceCache = ResourceCache::Get(); pResourceCache != nullptr)
    {
        m_pSourceTexture = pResourceCache->Load<ITexture>(pathToTexture.data());
        if (!m_pSourceTexture)
        {
            LOG(Error, "Unable to initialize sprite, ITexture was nullptr");
            return false;
        }

        m_textureSourceRect = sourceRect;
        m_drawParams = params;
        return true;
    }
    LOG(Error, "Unable to initialize sprite, Resource cache was nullptr");
    return false;
}

bool yang::Sprite::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    const char* pSourcePath = pData->Attribute("Src");
    if (pSourcePath)
    {
        m_pSourceTexture = ResourceCache::Get()->Load<ITexture>(pSourcePath);
        if (!m_pSourceTexture)
        {
            LOG(Error, "Failed to initialize sprite: texture failed to load");
            return false;
        }
    }
    else
    {
        LOG(Error, "Sprite needs to have source path to texture");
        return false;
    }

    XMLElement* pTint = pData->FirstChildElement("Tint");
    if (pTint)
    {
        IColor tint(pTint->IntAttribute("r"),
            pTint->IntAttribute("g"),
            pTint->IntAttribute("b"),
            pTint->IntAttribute("a"));

        m_pSourceTexture->SetTint(tint);
        m_pSourceTexture->SetAlpha(tint.Alpha());
    }

    XMLElement* pSourceRect = pData->FirstChildElement("SourceRect");
    if (pSourceRect)
    {
        m_textureSourceRect.x = pSourceRect->IntAttribute("x");
        m_textureSourceRect.y = pSourceRect->IntAttribute("y");
        m_textureSourceRect.width = pSourceRect->IntAttribute("width");
        m_textureSourceRect.height = pSourceRect->IntAttribute("height");
    }
    else
    {
        auto textureDimensions = m_pSourceTexture->GetDimensions();
        m_textureSourceRect.x = 0;
        m_textureSourceRect.y = 0;
        m_textureSourceRect.width = textureDimensions.x;
        m_textureSourceRect.height = textureDimensions.y;
        LOG(Warning, "No source rect for sprite specified. Using whole texture");
    }

    return true;
}
