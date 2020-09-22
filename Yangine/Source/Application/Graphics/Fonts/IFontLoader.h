#pragma once
#include <memory>
/** \file IFontLoader.h */
/** IFontLoader interface description */


//! \namespace yang Contains all Yangine code
namespace yang
{
    class IFont;
    class IResource;
    class IGraphics;
/** \class IFontLoader */
/** Interface for loading fonts */
class IFontLoader
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	IFontLoader();

    /// Loads font resource from raw data resource with given font size
    /// \param pResource - raw data resource to load font from
    /// \param fontSize - font size in points to initialize font
    /// \return shared pointer to a Font resource
    virtual std::shared_ptr<IFont> LoadFont(IResource* pResource, int fontSize) = 0;

    /// Initializes FontLoader with given graphics system
    /// \param pGraphics - pointer to a graphics system
    /// \return true if initialized successfully
    virtual bool Init(IGraphics* pGraphics) = 0;

	/** Default Destructor */
	virtual ~IFontLoader();


private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}