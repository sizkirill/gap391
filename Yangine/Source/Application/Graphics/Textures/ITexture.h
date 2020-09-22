#pragma once
#include <Utils/Math.h>
#include <Utils/Rectangle.h>
#include <Application/Resources/Resource.h>
/** \file ITexture.h */
/** Texture resource interface description */


//! \namespace yang Contains all Yangine code
namespace yang
{

class IColor;
class FColor;

/** \class ITexture */
/** Interface for the texture resource */
class ITexture : public IResource
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
    ITexture(IResource* pResource);

	/** Default Destructor */
	virtual ~ITexture();

    /// Get the underlying texture representation
    /// \return pointer to a native texture
	virtual void* GetNativeTexture() const = 0;

    /// Sets the texture tint color
    /// \param color - integer color to tint \see yang::IColor
    /// \return true if tint was successfully set
    virtual bool SetTint(const IColor& color) = 0;

    /// Sets the texture tint color
    /// \param color - float color to tint \see yang::FColor
    /// \return true if tint was successfully set
    virtual bool SetTint(const FColor& color) = 0;

    /// Sets the texture transparency
    /// \param alpha - integer in range 0-255 to set transparency
    /// \return true if transparency was successfully set
    virtual bool SetAlpha(ui8 alpha) = 0;

    /// Sets the texture transparency
    /// \param alpha - float in range [0.0 - 1.0] to set transparency
    /// \return true if transparency was successfully set
    virtual bool SetAlpha(float alpha) = 0;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	IVec2 m_textureDimensions;  ///< Texture width and height in pixels
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

protected:
	// --------------------------------------------------------------------- //
	// Protected Member Variables
	// --------------------------------------------------------------------- //
	float m_alpha;				///< Texture opacity;

	// --------------------------------------------------------------------- //
	// Protected Member Functions
	// --------------------------------------------------------------------- //

	// TODO: Consider adding a name to a protected constructor, so maybe later we can find textures by their names?
	/// Protected default constructor. Constructs an empty texture.
	ITexture();

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
    
    /// Get texture width and height
    /// \return IVec2 representing width and height. \see yang::Vector2
	IVec2 GetDimensions() const { return m_textureDimensions; }

    /// Sets texture's width and height
    /// \param dimensions - IVec2 that contains texture's width and height. \see yang::Vector2
	void SetDimensions(IVec2 dimensions) { m_textureDimensions = dimensions; }

	/// Get float value of opacity
	/// \return opacity in range [0.f, 1.0f]
	float GetAlpha() const { return m_alpha; }

};
}