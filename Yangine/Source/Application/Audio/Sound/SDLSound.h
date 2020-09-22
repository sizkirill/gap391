#pragma once
/** \file SDLSound.h */
/** Specialization of ISound interface for SDL */
#include <SDL/SDL_mixer.h>

#include ".\ISound.h"


//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class SDLSound */
/** SDL Sound resource */
class SDLSound
	: public ISound
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	
	/// Constructor
	/// \param pResource - raw data that contains sound chunk
	/// \param pSound - the loaded SDL sound chunk
	SDLSound(IResource* pResource, Mix_Chunk* pSound);

	/// Default Destructor
	~SDLSound();

	/// Get Mix_Chunk resource
	/// \return pointer to a Mix_Chunk
	virtual void* GetNativeSound() override final { return m_pSound; }


private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	Mix_Chunk* m_pSound;	///< Actual SDL Sound resource

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}