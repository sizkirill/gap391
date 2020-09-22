#pragma once
#include <Application/ApplicationLayer.h>
/** \file DungeonGenerator_AppLayer.h */
/** TODO: File Purpose */


/** \class DungeonGenerator_AppLayer */
/** TODO: Class Purpose */
class DungeonGenerator_AppLayer : public yang::ApplicationLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	DungeonGenerator_AppLayer() = default;

	/** Default Destructor */
	~DungeonGenerator_AppLayer() = default;

	std::unique_ptr<yang::IGameLayer> CreateGameLayer() override final;

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


};
