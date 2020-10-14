#pragma once
#include <Application/ApplicationLayer.h>
/** \file PerlinNoiseWorld_AppLayer.h */
/** TODO: File Purpose */


/** \class PerlinNoiseWorld_AppLayer */
/** TODO: Class Purpose */
class PerlinNoiseWorld_AppLayer : public yang::ApplicationLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	PerlinNoiseWorld_AppLayer() = default;

	/** Default Destructor */
	~PerlinNoiseWorld_AppLayer() = default;

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
