#pragma once
#include <memory>
#include <vector>
#include <optional>

#include <Utils/Matrix.h>
#include <Utils/Vector2.h>
#include <Utils/Rectangle.h>
#include <Utils/Color.h>
/** \file IGraphics.h */
/** Graphics system interface description */

//! \namespace yang Contains all Yangine code
namespace yang
{
    // Forward declarations
	class IWindow;
    class ITexture;
    class IResource;

/** \enum FlipDirection */
/** Specifies the flip configuration of TextureDrawParams */
enum class FlipDirection
{
	kNone,          ///< No flip
	kVertical,      ///< Flip vertically
	kHorizontal,    ///< Flip horizontally
	kMaxFlips       ///< Invalid value
};

/** \struct TextureDrawParams */
/** Specifies parameters of how the texture will be drawn */
struct TextureDrawParams
{
	float m_angle = 0;                              ///< Angle to rotate the texture
	std::optional<IVec2> m_pointToRotate = {};      ///< Optional of the rotation point. If not specified - defaulted to center of the texture rect
	FlipDirection m_flip = FlipDirection::kNone;    ///< How to flip the texture
};

/** \class IGraphics */
/** Interface for graphics system wrappers */
class IGraphics
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	IGraphics();

	/** Default Destructor */
	virtual ~IGraphics();

    /// Initialize the Graphics system
    /// \param pWindow - Window pointer to tie graphics to
    /// \return true if initialized successfully
	virtual bool Initialize(IWindow* pWindow) = 0;

    /// Clear background to specified color
    /// \param red - red component of the color - int in range [0-255]
    /// \param green - green component of the color - int in range [0-255]
    /// \param blue - blue component of the color - int in range [0-255]
    /// \param alpha - alpha component of the color - int in range [0-255]
    /// \return true if successful
	virtual bool StartDrawing(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) = 0;

    /// Finalize the frame by presenting to the window.
	virtual void EndDrawing() = 0;

    /// Load the texture from a raw data resource
    /// \param pResource - Pointer to raw data resource
    /// \return Shared pointer to texture resource
    virtual std::shared_ptr<ITexture> LoadTexture(IResource* pResource) = 0;

    /// Draw the texture at specified position
    /// \param pTexture - texture to draw
    /// \param position - position where to draw the texture. Defaulted to {0,0}
    /// \param drawParams - parameters to draw the texture. Defaulted to a default constructed TextureDrawParams. \see yang::IGraphics::TextureDrawParams. 
    /// \return true if successful
	virtual bool DrawTexture(ITexture* pTexture, IVec2 position = {0,0}, const TextureDrawParams& drawParams = TextureDrawParams{}) = 0;

    /// Draw the texture at specified position
    /// \param pTexture - texture to draw
    /// \param dest - rectangle on the screen to draw the texture to (in pixels). \see yang::Rectangle
    /// \param drawParams - parameters to draw the texture. Defaulted to a default constructed TextureDrawParams. \see yang::IGraphics::TextureDrawParams. 
    /// \return true if successful
	virtual bool DrawTexture(ITexture* pTexture, const IRect& dest, const TextureDrawParams& drawParams = TextureDrawParams{}) = 0;

    /// Draw the portion of a texture at specified position
    /// \param pTexture - texture to draw
    /// \param src - rectangle to use from the texture (in pixels). \see yang::Rectangle
    /// \param dest - rectangle on the screen to draw the texture to (in pixels). \see yang::Rectangle
    /// \param drawParams - parameters to draw the texture. Defaulted to a default constructed TextureDrawParams. \see yang::IGraphics::TextureDrawParams. 
    /// \return true if successful
    virtual bool DrawTexture(ITexture* pTexture, const IRect& src, const IRect& dest, const TextureDrawParams& drawParams = TextureDrawParams{}) = 0;

    /// Draw the rectangle borders on the screen
    /// \param rect - The rectangle to draw (in pixels)
    /// \param color - integer color to draw the borders
    /// \return true if successful
    bool DrawRect(const IRect& rect, const IColor& color);

    /// Draw a filled rectangle on the screen
    /// \param rect - The rectangle to draw (in pixels)
    /// \param color - integer color to fill the rectangle
    /// \return true if successful
    bool FillRect(const IRect& rect, const IColor& color);

    /// Draw a line on the screen
    /// \param start - Position of the start point in pixels
    /// \param end - Position of the end point in pixels
    /// \param color - Integer color of the line
    /// \return true if successful
    bool DrawLine(const IVec2& start, const IVec2& end, const IColor& color);

    /// Draw connected lines on the screen
    /// \param points - std::vector of point positions.
    /// \param color - Integer color of lines
    /// \return true if successful
	bool DrawLines(const std::vector<IVec2>& points, const IColor& color);

    /// Draw the rectangle borders on the screen. Floating point version.
    /// \param rect - The rectangle to draw (in pixels)
    /// \param color - integer color to draw the borders
    /// \return true if successful
	bool DrawRect(const FRect& rect, const IColor& color);

    /// Draw a filled rectangle on the screen. Floating point version.
    /// \param rect - The rectangle to draw (in pixels)
    /// \param color - integer color to fill the rectangle
    /// \return true if successful
	bool FillRect(const FRect& rect, const IColor& color);

    /// Draw a line on the screen. Floating point version.
    /// \param start - Position of the start point in pixels
    /// \param end - Position of the end point in pixels
    /// \param color - Integer color of the line
    /// \return true if successful
	bool DrawLine(const FVec2& start, const FVec2& end, const IColor& color);

    /// Draw connected lines on the screen. Floating point version.
    /// \param points - std::vector of point positions.
    /// \param color - Integer color of lines
    /// \return true if successful
	bool DrawLines(const std::vector<FVec2>& points, const IColor& color);

    /// Draw the rectangle borders on the screen. Version without color. The current renderer color is used to draw the rectangle.
    /// \param rect - The rectangle to draw (in pixels)
    /// \return true if successful
	virtual bool DrawRect(const IRect& rect) = 0;

    /// Draw a filled rectangle on the screen. Version without color. The current renderer color is used to draw the rectangle.
    /// \param rect - The rectangle to draw (in pixels)
    /// \return true if successful
	virtual bool FillRect(const IRect& rect) = 0;

    /// Draw a line on the screen. Version without color. The current renderer color is used to draw the line.
    /// \param start - Position of the start point in pixels
    /// \param end - Position of the end point in pixels
    /// \return true if successful
	virtual bool DrawLine(const IVec2& start, const IVec2& end) = 0;

    /// Draw connected lines on the screen. Version without color. The current renderer color is used to draw lines.
    /// \param points - std::vector of point positions.
    /// \return true if successful
	virtual bool DrawLines(const std::vector<IVec2>& points) = 0;

    /// Draw the rectangle borders on the screen. Colorless version of the floating point overload. The current renderer color is used to draw the rectangle.
    /// \param rect - The rectangle to draw (in pixels)
    /// \return true if successful
	virtual bool DrawRect(const FRect& rect) = 0;

    /// Draw a filled rectangle on the screen. Colorless version of the floating point overload. The current renderer color is used to draw the rectangle.
    /// \param rect - The rectangle to draw (in pixels)
    /// \return true if successful
	virtual bool FillRect(const FRect& rect) = 0;

    /// Draw a line on the screen. Colorless version of the floating point overload. The current renderer color is used to draw the line.
    /// \param start - Position of the start point in pixels
    /// \param end - Position of the end point in pixels
    /// \return true if successful
	virtual bool DrawLine(const FVec2& start, const FVec2& end) = 0;

    /// Draw connected lines on the screen. Colorless version of the floating point overload. The current renderer color is used to draw lines.
    /// \param points - std::vector of point positions.
    /// \return true if successful
	virtual bool DrawLines(const std::vector<FVec2>& points) = 0;

    /// Draw the circle on the screen
    /// \param center - Center of the circle position in pixels.
    /// \param radius - Radius of the circle in pixels.
    /// \param color - Color of the circle.
    /// \param segments - Number of line segments used to draw the circle. Defaulted to 16
    /// \return true if successful
	bool DrawCircle(const FVec2& center, float radius, const IColor& color, uint8_t segments = 16);

    /// Draw the polygon on the screen
    /// \param points - std::vector of polygon vertices positions (in pixels) to draw
    /// \param color - The color of the polygon
    /// \return true if successful
	bool DrawPolygon(const std::vector<FVec2>& points, const IColor& color);

    /// Draw the circle on the screen. Colorless version. The current renderer color is used to draw lines.
    /// \param center - Center of the circle position in pixels.
    /// \param radius - Radius of the circle in pixels.
    /// \param segments - Number of line segments used to draw the circle. Defaulted to 16
    /// \return true if successful
	virtual bool DrawCircle(const FVec2& center, float radius, uint8_t segments = 16) = 0;

    /// Draw the polygon on the screen. Colorless version. The current renderer color is used to draw lines.
    /// \param points - std::vector of polygon vertices positions (in pixels) to draw
    /// \return true if successful
	virtual bool DrawPolygon(const std::vector<FVec2>& points) = 0;

	/// Set the render target to a texture. Nullptr resets the render target to the screen
    /// \param pTarget - Texture to set the render target to.
    /// \return true if successful
	virtual bool SetRenderTarget(ITexture* pTarget) = 0;

    /// Set the render draw color.
    /// \param color - color to set.
    /// \return true if successful
	virtual bool SetDrawColor(const IColor& color) = 0;

    /// Create a texture with given dimensions
    /// \param dimensions - width and height in pixels of a new texture
    /// \return Shared pointer to a created texture
	virtual std::shared_ptr<ITexture> CreateTexture(IVec2 dimensions) = 0;

    /// Create the graphics system to use
    /// Right now only SDL is supported
    /// \return Unique pointer to the graphics system
	static std::unique_ptr<IGraphics> Create();

    /// Draw a filled triangle on the screen
    /// Note: That function is slow, so prefer to use for debug purposes only.
    /// \param points - Array of triangle vertices position in pixels.
    /// \param color - color to draw the triangle
    /// \return true if successful
	bool FillTriangle(FVec2 points[3], const IColor& color);

    /// Draw a filled circle on the screen
    /// Note: That function is slow, so prefer to use for debug purposes only.
    /// \param center - Center of the circle position in pixels.
    /// \param radius - Radius of the circle in pixels.
    /// \param segments - Number of circle segments used to draw the circle. Defaulted to 16
    /// \param color - color to draw the circle
    /// \return true if successful
	bool FillCircle(const FVec2& center, float radius, const IColor& color, uint8_t segments = 16);

    /// Draw a filled circle on the screen. Colorless version. The current renderer color is used to draw lines.
    /// Note: That function is slow, so prefer to use for debug purposes only.
    /// \param center - Center of the circle position in pixels.
    /// \param radius - Radius of the circle in pixels.
    /// \param segments - Number of circle segments used to draw the circle. Defaulted to 16
    /// \return true if successful
	virtual bool FillCircle(const FVec2& center, float radius, uint8_t segments = 16) = 0;

    /// Draw a filled triangle on the screen. Colorless version. The current renderer color is used to draw lines.
    /// Note: That function is slow, so prefer to use for debug purposes only.
    /// \param points - Array of triangle vertices position in pixels.
    /// \return true if successful
	virtual bool FillTriangle(FVec2 points[3]) = 0;

    /// Return the underlying renderer system, depending on the used library.
    /// \return Pointer to a native renderer
    virtual void* GetNativeRenderer() = 0;

protected:
	// --------------------------------------------------------------------- //
	// Protected Member Functions
	// --------------------------------------------------------------------- //
    Matrix m_cameraTransform;
    float m_cameraRotation;
    FVec2 m_cameraScaleFactors;
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
    void SetCameraTransform(Matrix cameraTransform) {m_cameraTransform = cameraTransform; }
    void SetCameraRotation(float angle) {m_cameraRotation = angle; }
    void SetCameraScaleFactors(FVec2 factors) {m_cameraScaleFactors = factors; }
};
}