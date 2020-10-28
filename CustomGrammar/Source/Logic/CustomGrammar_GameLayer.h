#pragma once

#include <Logic/IGameLayer.h>

/** \file CustomGrammar_GameLayer.h */
/** TODO: File Purpose */

/** \class CustomGrammar_GameLayer */
class CustomGrammar_GameLayer : public yang::IGameLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	CustomGrammar_GameLayer() = default;

	/** Default Destructor */
	~CustomGrammar_GameLayer() = default;

	const char* GetGameName() const override final { return "Mod 07 - Custom Grammar"; }
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
