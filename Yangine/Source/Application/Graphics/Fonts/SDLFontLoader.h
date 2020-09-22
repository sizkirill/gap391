#pragma once
/** \file SDLFontLoader.h */
/** Specialization of IFontLoader interface for SDL */

#include ".\IFontLoader.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
    class IGraphics;
    class SDLRenderer;
/** \class SDLFontLoader */
/** Loads TTF_Fonts */
class SDLFontLoader
	: public IFontLoader
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	SDLFontLoader();

	/** Default Destructor */
	~SDLFontLoader();

    /// Loads font resource from raw data resource with given font size
    /// \param pResource - raw data resource to load font from
    /// \param fontSize - font size in points to initialize font
    /// \return shared pointer to a Font resource
    virtual std::shared_ptr<IFont> LoadFont(IResource* pResource, int fontSize) override final;

    /// Initializes FontLoader with given graphics system
    /// \param pGraphics - pointer to SDL graphics system
    /// \return true if initialized successfully
    virtual bool Init(IGraphics* pGraphics) override final;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    SDLRenderer* m_pGraphics;   ///< SDL graphics system

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}