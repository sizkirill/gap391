#pragma once

#include <Logic/IGameLayer.h>
#include <Logic/Map/DungeonMap.h>
#include <string_view>

/** \file DungeonGenerator_GameLayer.h */
/** TODO: File Purpose */

/** \class DungeonGenerator_GameLayer */
class DungeonGenerator_GameLayer : public yang::IGameLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	DungeonGenerator_GameLayer() = default;

	/** Default Destructor */
	~DungeonGenerator_GameLayer() = default;

	const char* GetGameName() const override final { return kGameName; }
    virtual bool Init(const yang::ApplicationLayer& app) override final;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //

	static constexpr const char* kGameName = "DungeonGenerator";

	DungeonMap m_map;
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
