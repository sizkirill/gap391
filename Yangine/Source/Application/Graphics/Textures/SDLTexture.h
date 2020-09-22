#pragma once
/** \file SDLTexture.h */
/** Specialization of ITexture resource for SDL */

#include ".\ITexture.h"

#include <memory>

#include <SDL/SDL_render.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class SDLTexture */
/** SDL Texture resource */
class SDLTexture
	: public ITexture
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
	SDLTexture(IResource* pResource);

	/** Default Destructor */
	~SDLTexture();

    /// Get the underlying texture representation
    /// \return pointer to a SDL_Texture
	virtual void* GetNativeTexture() const override final;

    /// Initializes the texture resource
    /// \param pRenderer - pointer to a SDL_Renderer to associate the texture with
    /// \param pSurface - pointer to SDL_Surface to make the texture from
    /// \return true if initialized successfully
	bool Init(SDL_Renderer* pRenderer, SDL_Surface* pSurface);

    /// Sets the texture tint color
    /// \param color - integer color to tint \see yang::IColor
    /// \return true if tint was successfully set
    virtual bool SetTint(const IColor& color) override final;

    /// Sets the texture tint color
    /// \param color - float color to tint \see yang::FColor
    /// \return true if tint was successfully set
    virtual bool SetTint(const FColor& color) override final;

    /// Sets the texture transparency
    /// \param alpha - integer in range 0-255 to set transparency
    /// \return true if transparency was successfully set
    virtual bool SetAlpha(ui8 alpha) override final;

    /// Sets the texture transparency
    /// \param alpha - float in range [0.0 - 1.0] to set transparency
    /// \return true if transparency was successfully set
    virtual bool SetAlpha(float alpha) override final;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_pTexture;  ///< Actual SDL_Texture resource

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

	// Making SDLRenderer and SDLFont a friend, so it has access to private constructor. Inevitable evil.
	friend class SDLRenderer;
	friend class SDLFont;

	/// Private constructor
	/// \param pTexture - texture to construct with
	SDLTexture(SDL_Texture* pTexture);

	/// Private default constructor. To use in friend classes
	SDLTexture();

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

};
}