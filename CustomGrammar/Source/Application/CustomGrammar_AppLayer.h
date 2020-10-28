#pragma once
#include <Application/ApplicationLayer.h>
/** \file CustomGrammar_AppLayer.h */
/** TODO: File Purpose */


/** \class CustomGrammar_AppLayer */
/** TODO: Class Purpose */
class CustomGrammar_AppLayer : public yang::ApplicationLayer
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	CustomGrammar_AppLayer() = default;

	/** Default Destructor */
	~CustomGrammar_AppLayer() = default;

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
