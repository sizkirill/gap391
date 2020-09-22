#pragma once
#include <Utils/Math.h>
#include <memory>
/** \file SpriteComponent.h */
/** TODO: SpriteComponent class description */

#include <Application/Graphics/IGraphics.h>
#include ".\IComponent.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
	class LuaManager;
    class TransformComponent;
    class ITexture;
/** \class SpriteComponent */
/** Actor component that handles how the actor's sprite is rendered */
class SpriteComponent
	: public IComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param pOwner - actor, that owns this component
	SpriteComponent(Actor* pOwner);

	/** Default Destructor */
	virtual ~SpriteComponent();

    /// Name of this component, used for hashing
    /// \return "SpriteComponent"
    static constexpr const char* GetName() { return "SpriteComponent"; }

    /// Initialize the component from an XML element
    /// \param pData - XML Element that contains component data
    /// \return true if initialized successfully
    virtual bool Init(tinyxml2::XMLElement* pData) override;

    /// Obtains reference to actor's transform component
    /// \return true if actor has transform component
    virtual bool PostInit() override;

    /// Draw the texture centered at transform position
    /// \param pGraphics - graphics system to use
    /// \return true if render was successful
    virtual bool Render(IGraphics* pGraphics) override;

    /// Registers member functions to Lua environment
    /// Not intended for use outside of IGameLayer::Init
    /// \param luaManager - the Lua environment manager \see yang::LuaManager
	static void RegisterToLua(const LuaManager& luaManager);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    IRect m_textureSourceRect;              ///< Portion of the texture to draw
    IVec2 m_spriteDimensions;               ///< Dimensions with which the sprite will be drawn

	//TODO: Assign draw params at textures, not in the sprite component
	TextureDrawParams m_textureDrawParams;  ///< Texture draw parameters. \see yang::ITexture::TextureDrawParams

    TransformComponent* m_pTransform;       ///< Owner actor's transform component

	std::shared_ptr<ITexture> m_pTexture;   ///< Texture to render

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Get the source rect that defines the portion of image to draw
    const IRect& GetSourceRect() const { return m_textureSourceRect; }

    /// Get dimensions of the sprite to draw
    const IVec2& GetDimensions() const { return m_spriteDimensions; }

    /// Get the texture
    /// \return shared pointer to the texture
    std::shared_ptr<ITexture> GetTexture() const { return m_pTexture; }

    /// Set the source rect for the portion of the image to draw
    void SetSourceRect(const IRect& rect) { m_textureSourceRect = rect; }

    /// Set the dimensions of the sprite to draw
    void SetDimensions(const IVec2& dimensions) { m_spriteDimensions = dimensions; }

    /// Set the texture
    /// \param pTexture - shared pointer to a new texture
    void SetTexture(std::shared_ptr<ITexture> pTexture) { m_pTexture = pTexture; }

    /// Set rotation angle of the texture draw parameters
    /// \param angle - angle in radians
	void SetRotationAngle(float angle);

    /// Get the current rotation angle 
    /// \return angle in radians
	float GetRotationAngle() const;

    /// Set the texture rotation point
    /// \param point - position on the texture in pixels
	void SetRotationPoint(IVec2 point);

    /// Get the rotation point
    /// \return position of rotation point on the texture in pixes if it is initializes or empty optional
	std::optional<IVec2> GetRotationPoint() const;

    /// Set the texture flip
	void SetFlip(FlipDirection flip);

    /// Get the current texture flip
	FlipDirection GetFlip() const;

    /// Set the sprite direction (used in Lua environment) Basically sets the rotation point and appropriate angle
    /// \param fromPoint - start of the direction
    /// \param toPoint - end of the direction
	void SetDirection(FVec2 fromPoint, FVec2 toPoint);
};
}