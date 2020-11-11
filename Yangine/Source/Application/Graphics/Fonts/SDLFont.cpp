#include "SDLFont.h"
#include <cassert>
#include <Utils/Logger.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/Graphics/SDLRenderer.h>
#include <Application/Graphics/Textures/SDLTexture.h>
#include <Application/Graphics/Textures/Sprite.h>
#include <Application/Graphics/Fonts/FontString.h>

using yang::SDLFont;

SDLFont::SDLFont(IResource* pResource, TTF_Font* pFont, int ptSize)
    :IFont(pResource)
	,m_pFont(pFont)
    ,m_pFontAtlas(nullptr)
    ,m_pSDLRenderer(nullptr)
	,m_offset(0)
{
    m_filepath.append(std::to_string(ptSize));
}

bool yang::SDLFont::Init(IGraphics* pGraphics)
{
	// m_pFont should not be nullptr
	assert(m_pFont && "TTF_Font* was nullptr");

	// pGraphics should not be nullptr
	assert(pGraphics && "pGraphics was nullptr");

	int fontLineHeight = TTF_FontLineSkip(m_pFont);
	
	int textureDimension = 20 * fontLineHeight;

	m_pSDLRenderer = static_cast<SDLRenderer*>(pGraphics);

	m_pFontAtlas = m_pSDLRenderer->CreateTexture(IVec2(textureDimension, textureDimension));
	if (!m_pSDLRenderer->SetRenderTarget(m_pFontAtlas.get()))
	{
		// Graphics already logged an error
		return false;
	}

	int startX = 0;
	int startY = 0;

	// Hardcoded offset to 4
	m_offset = 4;

	m_glyphs.reserve(255-32);

	for (unsigned char c = 32; c < 255; ++c)
	{
		SDL_Surface* pGlyphSurface = TTF_RenderGlyph_Blended(m_pFont, c, { 255,255,255,255 });

		Glyph glyph;

		TTF_GlyphMetrics(m_pFont, c, &glyph.m_minX, nullptr, nullptr, &glyph.m_maxY, &glyph.m_advance);
	
		glyph.m_srcRect.width = pGlyphSurface->w;// +m_offset;
		glyph.m_srcRect.height = pGlyphSurface->h;// +m_offset;

		SDLTexture* pGlyphTexture = new SDLTexture();
		pGlyphTexture->Init(static_cast<SDL_Renderer*>(m_pSDLRenderer->GetNativeRenderer()), pGlyphSurface);

		if (startX + glyph.m_srcRect.width >= textureDimension)
		{
			startX = 0;
			startY += fontLineHeight + m_offset;
		}

		glyph.m_srcRect.x = startX;
		glyph.m_srcRect.y = startY;
		m_glyphs.push_back(glyph);

		SDL_FreeSurface(pGlyphSurface);

		m_pSDLRenderer->DrawTexture(pGlyphTexture, glyph.m_srcRect);

		startX += glyph.m_srcRect.width;
		delete pGlyphTexture;
	}

	m_pSDLRenderer->SetRenderTarget(nullptr);

    return true;
}

std::shared_ptr<yang::ITexture> yang::SDLFont::CreateTextureFromString(const std::string& str) const
{
	IVec2 textureDimensions;
	TTF_SizeText(m_pFont, str.c_str(), &textureDimensions.x, &textureDimensions.y);
	auto pTexture = m_pSDLRenderer->CreateTexture(textureDimensions + IVec2(m_offset, m_offset));
	m_pSDLRenderer->SetRenderTarget(pTexture.get());

	IRect dest{0,0,0,0};
	int currentX = 0;
	int currentY = 0;
	for (unsigned char c : str)
	{
		const Glyph& glyph = m_glyphs[static_cast<size_t>(c) - 32];
		dest.x = currentX + glyph.m_minX;
		dest.width = glyph.m_srcRect.width;
		dest.height = glyph.m_srcRect.height;

		m_pSDLRenderer->DrawTexture(m_pFontAtlas.get(), glyph.m_srcRect, dest);
		currentX += glyph.m_advance;
	}

	m_pSDLRenderer->SetRenderTarget(nullptr);

	return pTexture;
}

yang::FontString yang::SDLFont::CreateFontString(const std::string& str) const
{
	FontString result;
	result.m_text.reserve(str.size());
	result.m_pFont = shared_from_this();
	result.m_string = str;
	for (auto c : str)
	{
		result.m_text.emplace_back(SpriteFromChar(c));
	}

	return result;
}

std::shared_ptr<yang::Sprite> yang::SDLFont::SpriteFromChar(char c) const
{
	const Glyph& glyph = m_glyphs[static_cast<size_t>(c) - 32];
	return std::make_shared<Sprite>(m_pFontAtlas, glyph.m_srcRect);
}

int yang::SDLFont::GetXOffset(char c) const
{
	return m_glyphs[static_cast<size_t>(c) - 32].m_minX;
}

int yang::SDLFont::GetYOffset(char c) const
{
	return m_glyphs[static_cast<size_t>(c) - 32].m_maxY;
}

int yang::SDLFont::GetAdvance(char c) const
{
	return m_glyphs[static_cast<size_t>(c) - 32].m_advance;
}

yang::IVec2 yang::SDLFont::GetTextSize(const std::string& str) const
{
	IVec2 result;
	TTF_SizeText(m_pFont, str.c_str(), &result.x, &result.y);
	return result;
}

yang::IVec2 yang::SDLFont::GetTextSize(std::string_view str) const
{
	IVec2 result;
	TTF_SizeText(m_pFont, str.data(), &result.x, &result.y);
	return result;
}

SDLFont::~SDLFont()
{
	TTF_CloseFont(m_pFont);
	m_pFont = nullptr;
}