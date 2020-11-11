#pragma once
/** \file SDLFont.h */
/** Specialization of IFont resource for SDL */

#include ".\IFont.h"
#include <SDL/SDL_ttf.h>
#include <memory>
#include <vector>
#include <Utils/Math.h>
#include <Utils/Rectangle.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class SDLRenderer;
    class ITexture;
/** \class SDLFont */
/** SDL Font resource */
class SDLFont
	: public IFont
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    ///Constructor
    /// \param pResource - pointer to raw data resource
    /// \param pFont - pointer to loaded SDL Font
    /// \param ptSize - font size in points
	SDLFont(IResource* pResource, TTF_Font* pFont, int ptSize);

    /// Initializes the resource
    /// \param pGraphics - pointer to a SDLRenderer
    /// \return true if initialized successfully
    virtual bool Init(IGraphics* pGraphics) override final;

    /// Creates a texture, containing the given text
    /// \param str - string to create texture from
    /// \return shared pointer to a texture resource
	virtual std::shared_ptr<ITexture> CreateTextureFromString(const std::string& str) const override final;

	virtual FontString CreateFontString(const std::string& str) const override final;
	virtual std::shared_ptr<Sprite> SpriteFromChar(char c) const override final;
	virtual int GetXOffset(char c) const override final;
	virtual int GetYOffset(char c) const override final;
	virtual int GetAdvance(char c) const override final;
	virtual yang::IVec2 GetTextSize(const std::string& str) const override final;
	virtual yang::IVec2 GetTextSize(std::string_view str) const override final;

	/** Default Destructor */
	~SDLFont();

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	SDLRenderer* m_pSDLRenderer;            ///< SDLRenderer to create textures
	TTF_Font* m_pFont;                      ///< TTF_Font that is used to create textures
    std::shared_ptr<ITexture> m_pFontAtlas; ///< Texture containing chars from 32 to 127

	int m_offset;                           ///< Vertical offset between lines in the font atlas

    /// \struct Glyph
    /// Contains glyph data
	struct Glyph
	{
		IRect m_srcRect;                    ///< Rectangle representing position in the font atlas 
		int m_minX;                         ///< X offset for drawing
		int m_maxY;                         ///< Y offset for drawing
		int m_advance;                      ///< How far is the next character
	};

	std::vector<Glyph> m_glyphs;            ///< Vector containing all glyph data for the font

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

};
}