#pragma once

#include <Logic/IGameLayer.h>
#include <Logic/Map/WorldMap.h>
#include <string_view>

/** \file PerlinNoiseWorld_GameLayer.h */
/** TODO: File Purpose */

class CameraSystem;

/** \class PerlinNoiseWorld_GameLayer */
class PerlinNoiseWorld_GameLayer : public yang::IGameLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	PerlinNoiseWorld_GameLayer() = default;

	/** Default Destructor */
	~PerlinNoiseWorld_GameLayer() = default;

	const char* GetGameName() const override final { return kGameName; }
    virtual bool Init(const yang::ApplicationLayer& app) override final;
	virtual void Update(float deltaSeconds) override final;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	static constexpr const char* kGameName = "Mod 04 - Perlin Noise World";

	WorldMap m_map;
	std::shared_ptr<CameraSystem> m_pCameraSystem;
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
