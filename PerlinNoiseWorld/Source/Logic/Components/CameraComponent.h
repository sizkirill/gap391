#pragma once
#include <Utils/Vector2.h>
#include <Utils/Matrix.h>
#include <memory>
/** \file CameraComponent.h */
/** TODO: File Purpose */

#include <Logic/Components/IComponent.h>

namespace yang
{
	class TransformComponent;
}

class CameraSystem;

/** \class CameraComponent */
/** TODO: Class Purpose */
class CameraComponent
	: public yang::IComponent
{
	// Declaring CameraSystem a friend, so it can access this class private variables, and we don't need to write public getters/setters (that should be used only by CameraSystem anyway)
	friend class CameraSystem;
public:
	class ISpeedFunction
	{
	public:
		ISpeedFunction() = default;
		virtual float operator()(int distanceFromEdge) const = 0;
		virtual ~ISpeedFunction() = default;
	};

	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	CameraComponent(yang::Actor* pOwner);

	/// The name of this component
	/// \return "CameraComponent"
	static constexpr const char* GetName() {return "CameraComponent"; }

	/// Initializes CameraComponent from XMLElement
	/// \param pData - pointer to XMLElement to initialize CameraComponent from.
	/// \return true if initialized successfully
	virtual bool Init(tinyxml2::XMLElement* pData) override final;
	
	/// Post initializes the camera component
	/// \brief Gets the transform component from the owner camera actor
	/// \return true if post initialization was sucessful
	virtual bool PostInit() override final;

	/// \brief Sets camera focus on the actor
	/// \param pActor - actor to set focus on
	void SetFocusOnActor(yang::Actor* pActor);

	/// \brief Sets camera focus on location
	/// \param location - location to set focus on
	void SetFocusOnLocation(yang::FVec2 location);

	/// \brief Moves the camera by specified amount
	/// \param delta - amount to move the camera
	void MoveCamera(yang::FVec2 delta);

	float CalculateSpeed(int distanceFromEdge) const;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	float m_moveSpeed;									///< Maximum camera movement speed
	float m_senseDistance;								///< How far the cursor should be near the edge of the window in order for camera to move

	/// \brief Functor that determines how scrolling speed depends on distance from screen edge. 
	/// (distance from angle is between 0.0 and 1.0, where 0.0 is the edge, and 1.0 is sense distance)
	/// Returns a coefficient between 0.0 and 1.0 that will be multiplied by the maximum move speed
	std::unique_ptr<ISpeedFunction> m_pSpeedFunction;

	yang::TransformComponent* m_pCameraTransform;		///< Camera transform component;

	float m_startScale;									///< Camera start scale
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

	void SetSpeedFunction(ISpeedFunction* pSpeedFunction) { m_pSpeedFunction.reset(pSpeedFunction); }

	yang::Matrix GetCameraTransform() const;
	float GetCameraRotation() const;
	yang::FVec2 GetCameraScaleFactors() const;
};