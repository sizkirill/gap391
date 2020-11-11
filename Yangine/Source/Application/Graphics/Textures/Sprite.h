#pragma once
/** \file Sprite.h */
/** Sprite description (part of the texture)*/

#include <memory>
#include <Utils/Vector2.h>
#include <Utils/Rectangle.h>
#include <Application/Graphics/IGraphics.h>
#include <string_view>

namespace tinyxml2
{
    class XMLElement;
}

//! \namespace yang Contains all Yangine code
namespace yang
{
class ITexture;

class Sprite
{
public:
    Sprite() = default;
    Sprite(std::shared_ptr<ITexture> pTexture, IRect sourceRect, TextureDrawParams params = {});
    bool Init(std::string_view pathToTexture, IRect sourceRect, TextureDrawParams params);
    bool Init(tinyxml2::XMLElement* pData);
private:
    std::shared_ptr<ITexture> m_pSourceTexture = nullptr;
    IRect m_textureSourceRect;
    TextureDrawParams m_drawParams;
public:
    IRect GetSourceRect() const { return m_textureSourceRect; }
    void SetSourceRect(IRect sourceRect) { m_textureSourceRect = sourceRect; }
    TextureDrawParams& GetDrawParams() { return m_drawParams; }
    std::weak_ptr<ITexture> GetSourceTexture() const { return m_pSourceTexture; }
};

}