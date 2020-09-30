#pragma once
/** \file ParticleEmitterComponent.h */
/** TODO: File Purpose */

#include <Logic/Components/IComponent.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Utils/Vector2.h>
#include <Utils/Random.h>
#include <Utils/Color.h>
#include <vector>
#include <variant>

//! \namespace yang Contains all Yangine code
namespace yang
{
	class Sprite;
	class TransformComponent;

/** \class ParticleEmitterComponent */
/** TODO: Class Purpose */
class ParticleEmitterComponent
	: public yang::IComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	ParticleEmitterComponent(yang::Actor* pOwner);

	/// The name of this component
	/// \return "ParticleEmitterComponent"
	static constexpr const char* GetName() {return "ParticleEmitterComponent"; }

	/// Initializes ParticleEmitterComponent from XMLElement
	/// \param pData - pointer to XMLElement to initialize ParticleEmitterComponent from.
	/// \return true if initialized successfully
	virtual bool Init(tinyxml2::XMLElement* pData) override final;
	
	// TODO(ksizykh): move it to a system or a process
	/// Updates this component by deltaSeconds time
	/// \param deltaSeconds - time since last frame
	virtual void Update(float deltaSeconds) override final;

	/// <summary>
	/// Renders particles
	/// </summary>
	/// <param name="pGraphics"> graphics system to use</param>
	/// <returns>true if rendered successfully</returns>
	virtual bool Render(yang::IGraphics* pGraphics) override final;

	/// <summary>
	/// Post initializes the component (gets the pointer to an actor's transform component)
	/// </summary>
	/// <returns>true if owner actor has transform</returns>
	virtual bool PostInit() override final;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	struct Particle
	{
		FVec2 m_positionOffset = { 0,0 };	///< particle's offset relative to center of the emitter
		FVec2 m_velocity = { 0,0 };			///< particle velocity
	};

	FVec2 m_centerOffset = { 0,0 };										  ///< Offset relative to center of the owner actor
	FVec2 m_size = { 0,0 };												  ///< Particle size
	size_t m_count = 0;													  ///< Max number of spawned particles
	float m_radiusSqrd = 0;												  ///< Square of maximum distance for particles to travel (do we need it?)
	std::vector<Particle> m_particles = {};								  ///< Container for particle instances
	XorshiftRNG m_rngDevice = XorshiftRNG();										  ///< RNG device for particles
	std::variant<std::shared_ptr<Sprite>, IColor> m_drawable = nullptr;   ///< Texture or color to draw
	float m_lifetime = 0;												  ///< Lifetime of a particle system
	FVec2 m_speedRange = { 0,0 };										  ///< Range of starting particle speeds
	FVec2 m_initialAngleRange = { 0,0 };								  ///< Range of initial spawn angle

	yang::TransformComponent* m_pOwnerTransform = nullptr;
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}