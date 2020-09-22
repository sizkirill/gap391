#include "SDLFontLoader.h"
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rwops.h>

#include <Utils/Logger.h>
#include <Application/Resources/Resource.h>
#include <Application/Graphics/Fonts/SDLFont.h>
#include <Application/Graphics/SDLRenderer.h>
using yang::SDLFontLoader;

SDLFontLoader::SDLFontLoader()
	:m_pGraphics(nullptr)
{
	
}

SDLFontLoader::~SDLFontLoader()
{
    TTF_Quit();
}

std::shared_ptr<yang::IFont> yang::SDLFontLoader::LoadFont(IResource* pResource, int fontSize)
{
	SDL_RWops* pOps = SDL_RWFromConstMem(pResource->GetData().data(), static_cast<int>(pResource->GetData().size()));
	TTF_Font* pFont = TTF_OpenFontRW(pOps, 0, fontSize);
	TTF_SetFontKerning(pFont, 0);

	if (!pFont)
	{
		LOG(Error, "Unable to load font. Error: %s", SDL_GetError());
		return nullptr;
	}

    SDLFont* pSDLFont = new SDLFont(pResource, pFont, fontSize);

    if (!pSDLFont->Init(m_pGraphics))
    {
        LOG(Error, "Failed to initialize font. Error: %s", SDL_GetError());
        delete pSDLFont;
        return nullptr;
    }

	return std::shared_ptr<SDLFont>(pSDLFont);
}

bool yang::SDLFontLoader::Init(IGraphics* pGraphics)
{
    if (TTF_Init() != 0)
    {
        LOG(Error, "Failed to initialize TTF: %s", TTF_GetError());
        return false;
    }
    m_pGraphics = static_cast<SDLRenderer*>(pGraphics);
    return true;
}
