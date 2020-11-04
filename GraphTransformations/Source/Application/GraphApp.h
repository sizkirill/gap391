#pragma once
#include <Application/ApplicationLayer.h>
/** \file GraphApp.h */
/** TODO: File Purpose */


/** \class GraphApp */
/** TODO: Class Purpose */
class GraphApp : public yang::ApplicationLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	GraphApp() = default;

	/** Default Destructor */
	~GraphApp() = default;

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
