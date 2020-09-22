#pragma once
/** \file SDLRenderer.h */
/** SDL Renderer graphics */

#include ".\IGraphics.h"
#include <SDL/SDL_render.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class SDLRenderer */
/** SDL_Renderer wrapper for IGraphics interface specialization */
class SDLRenderer
	: public IGraphics
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	SDLRenderer();

	/** Default Destructor */
	~SDLRenderer();

    /// Initialize the SDL_Renderer
    /// \param pWindow - Window pointer to tie graphics to. Assumes that underlying IWindow representation is SDL_Window
    /// \return true if initialized successfully
	virtual bool Initialize(IWindow* pWindow) override final;

    /// Clear background to specified color
    /// \param red - red component of the color - int in range [0-255]
    /// \param green - green component of the color - int in range [0-255]
    /// \param blue - blue component of the color - int in range [0-255]
    /// \param alpha - alpha component of the color - int in range [0-255]
    /// \return true if successful
	virtual bool StartDrawing(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) override final;

    /// Finalize the frame by presenting to the window.
	virtual void EndDrawing() override final;

    /// Load the SDL_Texture from a raw data resource
    /// \param pResource - Pointer to raw data resource
    /// \return Shared pointer to texture resource
    virtual std::shared_ptr<ITexture> LoadTexture(IResource* pResource) override final;

    /// Draw the texture at specified position
    /// \param pTexture - texture to draw
    /// \param position - position where to draw the texture. Defaulted to {0,0}
    /// \param drawParams - parameters to draw the texture. Defaulted to a default constructed TextureDrawParams. \see yang::IGraphics::TextureDrawParams. 
    /// \return true if successful
	virtual bool DrawTexture(ITexture* pTexture, IVec2 position = { 0,0 }, const TextureDrawParams & drawParams = {}) override final;

    /// Draw the texture at specified position
    /// \param pTexture - texture to draw
    /// \param dest - rectangle on the screen to draw the texture to (in pixels). \see yang::Rectangle
    /// \param drawParams - parameters to draw the texture. Defaulted to a default constructed TextureDrawParams. \see yang::IGraphics::TextureDrawParams. 
    /// \return true if successful
	virtual bool DrawTexture(ITexture* pTexture, const IRect& dest, const TextureDrawParams& drawParams = {}) override final;

    /// Draw the portion of a texture at specified position
    /// \param pTexture - texture to draw
    /// \param src - rectangle to use from the texture (in pixels). \see yang::Rectangle
    /// \param dest - rectangle on the screen to draw the texture to (in pixels). \see yang::Rectangle
    /// \param drawParams - parameters to draw the texture. Defaulted to a default constructed TextureDrawParams. \see yang::IGraphics::TextureDrawParams. 
    /// \return true if successful
	virtual bool DrawTexture(ITexture* pTexture, const IRect& src, const IRect& dest, const TextureDrawParams& drawParams = {}) override final;

    /// Draw the rectangle borders on the screen
    /// \param rect - The rectangle to draw (in pixels)
    /// \return true if successful
    virtual bool DrawRect(const IRect& rect) override final;

    /// Draw a filled rectangle on the screen
    /// \param rect - The rectangle to draw (in pixels)
    /// \return true if successful
    virtual bool FillRect(const IRect& rect) override final;

    /// Draw a line on the screen
    /// \param start - Position of the start point in pixels
    /// \param end - Position of the end point in pixels
    /// \return true if successful
    virtual bool DrawLine(const IVec2& start, const IVec2& end) override final;

    /// Draw connected lines on the screen
    /// \param points - std::vector of point positions.
    /// \return true if successful
	virtual bool DrawLines(const std::vector<IVec2>& points) override final;

    /// Draw the rectangle borders on the screen. Floating point version.
    /// \param rect - The rectangle to draw (in pixels)
    /// \return true if successful
	virtual bool DrawRect(const FRect& rect) override final;

    /// Draw a filled rectangle on the screen. Floating point version.
    /// \param rect - The rectangle to draw (in pixels)
    /// \return true if successful
	virtual bool FillRect(const FRect& rect) override final;

    /// Draw a line on the screen. Floating point version.
    /// \param start - Position of the start point in pixels
    /// \param end - Position of the end point in pixels
    /// \return true if successful
	virtual bool DrawLine(const FVec2& start, const FVec2& end) override final;

    /// Draw connected lines on the screen. Floating point version.
    /// \param points - std::vector of point positions.
    /// \return true if successful
	virtual bool DrawLines(const std::vector<FVec2>& points) override final;

    /// Draw the circle on the screen
    /// \param center - Center of the circle position in pixels.
    /// \param radius - Radius of the circle in pixels.
    /// \param segments - Number of line segments used to draw the circle. Defaulted to 16
    /// \return true if successful
	virtual bool DrawCircle(const FVec2& center, float radius, uint8_t segments) override final;

    /// Draw the polygon borders on the screen
    /// \param points - std::vector of polygon vertices positions (in pixels) to draw
    /// \return true if successful
	virtual bool DrawPolygon(const std::vector<FVec2>& points) override final;

    /// Set the render target to a texture. Nullptr resets the render target to the screen
    /// \param pTarget - Texture to set the render target to.
    /// \return true if successful
	virtual bool SetRenderTarget(ITexture* pTarget) override final;

    /// Set the render draw color.
    /// \param color - color to set.
    /// \return true if successful
	virtual bool SetDrawColor(const IColor& color) override final;

    /// Create a texture with given dimensions
    /// \param dimensions - width and height in pixels of a new texture
    /// \return Shared pointer to a created texture
	virtual std::shared_ptr<ITexture> CreateTexture(IVec2 dimensions) override final;

    /// Return the underlying renderer system.
    /// \return Pointer to the SDL_Renderer
    virtual void* GetNativeRenderer() override final { return m_pRenderer.get(); }

    /// Draw a filled triangle on the screen
    /// Note: That function is slow, so prefer to use for debug purposes only.
    /// \param points - Array of triangle vertices position in pixels.
    /// \return true if successful
    virtual bool FillTriangle(FVec2 points[3]) override final;

    /// Draw a filled circle on the screen
    /// Note: That function is slow, so prefer to use for debug purposes only.
    /// \param center - Center of the circle position in pixels.
    /// \param radius - Radius of the circle in pixels.
    /// \param segments - Number of circle segments used to draw the circle. Defaulted to 16
    /// \return true if successful
    virtual bool FillCircle(const FVec2& center, float radius, uint8_t segments) override final;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_pRenderer;  ///< Unique pointer to the SDL_Renderer

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    /// Helper function to convert rectangle to SDL_Rect. Truncates floating point values
    /// \param rect - rectangle to convert
    /// \return corresponding SDL_Rect
    template <class T>
    SDL_Rect ToSDLRect(const Rectangle<T>& rect);

    /// Helper function to convert rectangle to SDL_Rect.
    /// \param rect - rectangle to convert
    /// \return corresponding SDL_Rect
	template <class T>
	SDL_FRect ToSDLFRect(const Rectangle<T>& rect);

	/// Helper function to convert FlipDirection to SDL_RendererFlip. \see FlipDirection
    /// \param flip - FlipDirection to convert
    /// \return corresponding SDL_RendererFlip
	SDL_RendererFlip ToRendererFlip(FlipDirection flip);
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

};
template<class T>
inline SDL_Rect SDLRenderer::ToSDLRect(const Rectangle<T>& rect)
{
    return {
        static_cast<i32>(rect.x),
        static_cast<i32>(rect.y),
        static_cast<i32>(rect.width),
        static_cast<i32>(rect.height)
    };
}
template<class T>
inline SDL_FRect SDLRenderer::ToSDLFRect(const Rectangle<T>& rect)
{
	return
	{
		static_cast<f32>(rect.x),
		static_cast<f32>(rect.y),
		static_cast<f32>(rect.width),
		static_cast<f32>(rect.height)
	};
}
}