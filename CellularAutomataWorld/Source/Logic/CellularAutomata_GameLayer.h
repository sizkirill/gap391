#pragma once

#include <Logic/IGameLayer.h>
#include <Logic/Map/CellularWorldMap.h>

/** \file CellularAutomata_GameLayer.h */
/** TODO: File Purpose */

/** \class CellularAutomata_GameLayer */
class CellularAutomata_GameLayer : public yang::IGameLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	CellularAutomata_GameLayer() = default;

	/** Default Destructor */
	~CellularAutomata_GameLayer() = default;

	const char* GetGameName() const override final { return "Mod05 - Cellular Automata Simple World"; }
    virtual bool Init(const yang::ApplicationLayer& app) override final;
	virtual void Update(float deltaSeconds) override final;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	CellularWorldMap m_map;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
