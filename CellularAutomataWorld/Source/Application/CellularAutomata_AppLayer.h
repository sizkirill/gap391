#pragma once
#include <Application/ApplicationLayer.h>
/** \file CellularAutomata_AppLayer.h */
/** TODO: File Purpose */


/** \class CellularAutomata_AppLayer */
/** TODO: Class Purpose */
class CellularAutomata_AppLayer : public yang::ApplicationLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	CellularAutomata_AppLayer() = default;

	/** Default Destructor */
	~CellularAutomata_AppLayer() = default;

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
