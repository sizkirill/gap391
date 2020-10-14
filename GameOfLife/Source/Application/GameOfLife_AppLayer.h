#pragma once
#include <Application/ApplicationLayer.h>
/** \file GameOfLife_AppLayer.h */
/** TODO: File Purpose */


/** \class GameOfLife_AppLayer */
/** TODO: Class Purpose */
class GameOfLife_AppLayer : public yang::ApplicationLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	GameOfLife_AppLayer() = default;

	/** Default Destructor */
	~GameOfLife_AppLayer() = default;

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
