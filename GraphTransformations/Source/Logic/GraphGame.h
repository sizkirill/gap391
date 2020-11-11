#pragma once

#include <Logic/IGameLayer.h>

/** \file GraphGame.h */
/** TODO: File Purpose */

/** \class GraphGame */
class GraphGame : public yang::IGameLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	GraphGame() = default;

	/** Default Destructor */
	~GraphGame() = default;

	const char* GetGameName() const override final { return "Mod 08 - GraphTransformations"; }
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
