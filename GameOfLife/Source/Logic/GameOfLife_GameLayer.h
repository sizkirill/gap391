#pragma once

#include <Logic/IGameLayer.h>
#include <Logic/Map/GameOfLifeMap.h>

/** \file GameOfLife_GameLayer.h */
/** TODO: File Purpose */

/** \class GameOfLife_GameLayer */
class GameOfLife_GameLayer : public yang::IGameLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	GameOfLife_GameLayer() = default;

	/** Default Destructor */
	~GameOfLife_GameLayer() = default;

	const char* GetGameName() const override final { return "Mod05 - Conway's Game of Life"; }
    virtual bool Init(const yang::ApplicationLayer& app) override final;
	virtual void Update(float deltaSeconds) override final;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	GameOfLifeMap m_map;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
