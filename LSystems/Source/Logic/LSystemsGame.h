#pragma once

#include <Logic/IGameLayer.h>

/** \file LSystemsGame.h */
/** TODO: File Purpose */

/** \class LSystemsGame */
class LSystemsGame : public yang::IGameLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	LSystemsGame() = default;

	/** Default Destructor */
	~LSystemsGame() = default;

	const char* GetGameName() const override final { return "Mod 06 - L Systems"; }
    virtual bool Init(const yang::ApplicationLayer& app) override final;

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
