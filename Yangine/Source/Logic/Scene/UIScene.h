#pragma once
/** \file UIScene.h */
/** UIScene class description */
#include <vector>
#include <memory>
#include <Utils/Math.h>
#include <Utils/Vector2.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
	class Actor;
	class IGraphics;

// TODO: rename to UIElement?

/** \class UIScene */
/// Class that handles UI screens layout and rendering
class UIScene
{
public:
	/// \enum VerticalAlignment
	/// Represents the vertical alignment type
	enum class VerticalAlignment
	{
		kTop,  ///< Align top
		kDown, ///< Align down
		kNone  ///< Don't alignt
	};

	/// \enum HorizontalAlignment
    /// Represents the horizontal alignment type
	enum class HorizontalAlignment
	{
		kLeft,  ///< Align left
		kRight, ///< Align right
		kNone   ///< Don't align
	};

	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	UIScene();

	/// Constructor
	/// \param dimensions - width and height for the scene
	/// \param pParent - pointer to a parent of the new scene
	UIScene(FVec2 dimensions, UIScene* pParent = nullptr);

	/** Default Destructor */
	~UIScene();

	/// Adds a child scene with specified dimensions
	/// \param dimensions - scene dimensions
	/// \return pointer to a new scene
	UIScene* AddScene(FVec2 dimensions);

	/// Adds actor to a current scene.
	/// \param pActor - shared ptr to an actor
	void AddActor(std::shared_ptr<Actor> pActor);

	/// Aligns children scenes and contained actors
	/// \param vAlign - vertical alignment \see VerticalAlignment
	/// \param hAlign - horizontal alignment \see HorizontalAlignment
	void Align(VerticalAlignment vAlign, HorizontalAlignment hAlign);

	/// Render function, used for debug purposes. Renders borders of this scene and all children for debug purposes. Active only in debug mode
	/// \param pGraphics - pointer to Graphics system
	void Render(IGraphics* pGraphics);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	UIScene* m_pParent;                                 ///< Parent scene of this element. If it is null - this scene is a root element
	std::vector<std::unique_ptr<UIScene>> m_children;   ///< Vector of unique pointers to children elements
	std::vector<std::shared_ptr<Actor>> m_uiActors;     ///< Vector of shared pointers to actors that this UI Element contains

	FVec2 m_dimensions;                                 ///< Dimensions of this element
	FVec2 m_relativePosition;                           ///< Position relative to the root element

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

	/// Internal render function, recursively calls all children internal render
	/// \param pGraphics - graphics system to render
	/// \param parentPos - absolute position of the scene
	void InternalRender(IGraphics* pGraphics, FVec2 parentPos);
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	/// Dimensions getter
	/// \return IVec2 that represents scene's dimensions.
	FVec2 GetDimensions() const { return m_dimensions; }

	/// Dimensions getter
	/// \return IVec2 reference that represents scene's dimensions.
	FVec2& GetDimensions() { return m_dimensions; }
	
	/// Relative position getter
	/// \return FVec2, that is position relative to top left corner of the parent
	FVec2 GetRelativePosition() const { return m_relativePosition; }

	/// Sets this scene's relative position and properly aligns objects and children scenes
	/// \param relativePosition - the new relativePosition;
	void SetRelativePosition(FVec2 relativePosition);
};
}