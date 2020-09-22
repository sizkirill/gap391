#pragma once
/** \file TextComponent.h */
/** TextComponent class description */

#include ".\IComponent.h"
#include <memory>
#include <string>
#include <Utils/Math.h>
#include <Utils/Vector2.h>
#include <Utils/Color.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
	class IFont;
	class ITexture;
	class TransformComponent;
/** \class TextComponent */
/** Actor component that handles the text data and rendering */
class TextComponent
	: public IComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	
	/// TextComponent constructor
	/// \param pOwner - Actor who owns this component
	TextComponent(Actor* pOwner);

	/// Default Destructor
	~TextComponent();

	/// The name of this component
	/// \return "TextComponent"
	static constexpr const char* GetName() { return "TextComponent"; }

	/// Initializes TextComponent from XMLElement
	/// \param pData - pointer to XMLElement to initialize TextComponent from.
	/// \return true if initialized successfully
	virtual bool Init(tinyxml2::XMLElement* pData) override final;

	/// Post initializes the text component (gets the pointer to actor's transform component)
	/// return true if transform component found
	virtual bool PostInit() override final;

	/// Renders the text to screen
	/// \param pGraphics - graphics that will draw the text
	/// \return true if rendered successfully
	virtual bool Render(IGraphics* pGraphics) override final;

	/// Updates the text of this component
	/// \param text - new text to draw
	void UpdateText(const std::string& text);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	std::shared_ptr<IFont> m_pFont;			///< Font to use
	std::shared_ptr<ITexture> m_pTexture;	///< Texture that contains text
	TransformComponent* m_pTransform;		///< TransformComponent of the owner actor
	FVec2 m_relativePosition;				///< Position relative to the owning actor transform
	IColor m_textColor;					    ///< Color to render the text in
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	/// Getter for texture dimensions
	/// \return IVec2 that contains width and height of the contained texture
	IVec2 GetTextureDimensions() const;

	/// Getter for relative position
	/// \return text position relative to the actor's owner transform
	FVec2 GetRelativePosition() const { return m_relativePosition; }

	/// Sets the text color
	/// \param color - new color
	void SetColor(IColor color);
};
}