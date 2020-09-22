#include "SDLTexture.h"

#include <Utils/Logger.h>
#include <Utils/Color.h>

using yang::SDLTexture;

SDLTexture::SDLTexture(IResource* pResource)
	: ITexture(pResource)
    , m_pTexture(nullptr, &SDL_DestroyTexture)
{
	
}

SDLTexture::~SDLTexture()
{
	
}

void* yang::SDLTexture::GetNativeTexture() const
{
	return m_pTexture.get();
}

bool yang::SDLTexture::Init(SDL_Renderer* pRenderer, SDL_Surface* pSurface)
{
	m_pTexture.reset(SDL_CreateTextureFromSurface(pRenderer, pSurface));
	SetDimensions({ pSurface->w, pSurface->h });
	if (!m_pTexture)
	{
		LOG(Error, "SDL could not load texture: %s", SDL_GetError());
		return false;
	}

    SDL_SetTextureBlendMode(m_pTexture.get(), SDL_BlendMode::SDL_BLENDMODE_BLEND);

	return true;
}

bool yang::SDLTexture::SetTint(const IColor& color)
{
    if (SDL_SetTextureColorMod(m_pTexture.get(), color.Red(), color.Green(), color.Blue()))
    {
        LOG(Error, "Unable to tint texture. Error: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool yang::SDLTexture::SetTint(const FColor& color)
{
    IColor tintColor = color.ToIColor();
    return SetTint(tintColor);
}

bool yang::SDLTexture::SetAlpha(ui8 alpha)
{
    if (alpha == m_alpha * std::numeric_limits<ui8>::max())
    {
        return true;
    }
    if (SDL_SetTextureAlphaMod(m_pTexture.get(), alpha))
    {
        LOG(Error, "Unable to set alpha to a texture. Error: %s", SDL_GetError());
        return false;
    }
    m_alpha = static_cast<float>(alpha) / std::numeric_limits<ui8>::max();
    return true;
}

bool yang::SDLTexture::SetAlpha(float alpha)
{
    ui8 alphaU8 = static_cast<ui8>(alpha * std::numeric_limits<ui8>::max());
    return SetAlpha(alphaU8);
}

yang::SDLTexture::SDLTexture(SDL_Texture* pTexture)
	:ITexture()
	,m_pTexture(pTexture, &SDL_DestroyTexture)
{
}

yang::SDLTexture::SDLTexture()
	:ITexture()
	, m_pTexture(nullptr, &SDL_DestroyTexture)
{
}
