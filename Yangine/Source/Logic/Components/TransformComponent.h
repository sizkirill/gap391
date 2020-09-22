#pragma once
/** \file TransformComponent.h */
/** TransformComponent class description */

#include ".\IComponent.h"
#include <Utils/Math.h>
#include <Utils/Vector2.h>
#include <Utils/Matrix.h>

#include <Utils/StringHash.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
	class LuaManager;
/** \class TransformComponent */
/// Component that represents actor's position */
class TransformComponent
	: public IComponent
{
public:
	/// \enum TransformType
	/// Enum class that tells about type of this transform
	enum class TransformType
	{
		kWorld,		///< Transform is in the world coordinates
		kScreen,	///< Transform is in the screen coordinates
		kRelative,  ///< Transform is relative to the other transform. Requires pointer to other transform
		kMaxTypes   ///< Invalid transform type
	};
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param pOwner - Actor who owns this component
	TransformComponent(Actor* pOwner);

	/** Default Destructor */
	virtual ~TransformComponent();

    /// Initializes TextComponent from XMLElement
    /// \param pData - pointer to XMLElement to initialize TextComponent from.
    /// \return true if initialized successfully
    virtual bool Init(tinyxml2::XMLElement* pData) override final;

    /// The name of this component
    /// \return "TransformComponent"
    static constexpr const char* GetName() { return "TransformComponent"; }

    /// Move the actor component by specified amount (effectively teleports by that amount)
    /// \param dx - amount of movement on X axis
    /// \param dy - amount of movement on Y axis
	void Move(float dx, float dy);

	/// Move the actor component by specified amount (effectively teleports by that amount)
	/// \param offset - offset to move
	void Move(FVec2 offset);

	/// Scale actor by some amount in both axis
	/// \param amount - amount to scale by
	void Scale(float amount);

	/// Scale actor by amount.x in x axis and amount.y in y axis
	/// \param amount - amount to scale by
	void Scale(FVec2 amount);

	/// Rotate transform by specified amount in radians
	/// \param angle - angle in radians to move
	void Rotate(float angle);

    /// Registers member functions to Lua environment
    /// Not intended for use outside of IGameLayer::Init
    /// \param manager - the Lua environment manager \see yang::LuaManager
	static void RegisterToLua(const LuaManager& manager);

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    FVec2 m_position;                   ///< Actor's position
	FVec2 m_rotationPoint;				///< Transform rotation point
	float m_rotationAngle;				///< Actor's rotation (in radians, clockwise from initial point. 0 is considered pointing to the right)
	FVec2 m_scale;						///< Actor's scale
	TransformType m_transformType;      ///< Type of position. \see yang::TransformComponent::TransformType

	Matrix m_transformMatrix;			///< Matrix representing the current transform
	bool m_transformNeedUpdate;			///< Does transform matrix need update?
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

	void UpdateTransformMatrix();

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	/// Get the reference to current transform matrix
	Matrix& GetCurrentTransform();

	/// Does the transform matrix need update?
	/// \return true if transform matrix needs update
	bool MatrixNeedUpdate() const { return m_transformNeedUpdate; }

    /// Get the reference to actor's position. Can be modified
    FVec2& GetPosition() { return m_position; }

    /// Get the const reference to actor's position. Can't be modified.
    const FVec2& GetPosition() const { return m_position; }

    /// Set actor position
    /// \param position - new actor's position
	void SetPosition(FVec2 position);

    /// Get the type of position \see yang::TransformComponent::TransformType
	TransformType GetTransformType() const { return m_transformType; }

    /// Set the position type \see yang::TransformComponent::TransformType
	void SetTransformType(TransformType type) { m_transformType = type; }

    /// Get width and height of the actor. Internally accesses the TextComponent and SpriteComponent of the owner actor if they're present.
	FVec2 GetDimensions() const;

	float GetRotation() const { return m_rotationAngle; }

	void SetRotation(float angle);

	FVec2 GetRotationPoint() const { return m_rotationPoint; }

	void SetRotationPoint(FVec2 rotationPoint) { m_rotationPoint = rotationPoint; }

	FVec2 GetScaleFactors() const {return m_scale;}
};

}