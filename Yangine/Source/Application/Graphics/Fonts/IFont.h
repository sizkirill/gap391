#pragma once
/** \file IFont.h */
/** Font Resource */

#include "..\..\Resources\Resource.h"
#include <memory>
#include <string>

//! \namespace yang Contains all Yangine code
namespace yang
{
	class IGraphics;
	class ITexture;
/** \class IFont */
/** Interface for the Font resource */
class IFont
	: public IResource
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	
    /// Constructor
    /// \param pResource - pointer to a raw resource data
	IFont(IResource* pResource);

	/** Default Destructor */
	virtual ~IFont();

    /// Initializes the resource
    /// \param pGraphics - Graphics system to initialize the font from
    /// \return true if initialized successfully
	virtual bool Init(IGraphics* pGraphics) = 0;

    /// Creates a texture, containing the given text
    /// \param str - string to create texture from
    /// \return shared pointer to a texture resource
	virtual std::shared_ptr<ITexture> CreateTextureFromString(const std::string& str) = 0;

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