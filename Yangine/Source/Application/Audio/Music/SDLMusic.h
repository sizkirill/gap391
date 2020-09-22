#pragma once
/** \file SDLMusic.h */
/** TODO: Specialization of IMusic interface for SDL */
#include <SDL/SDL_mixer.h>

#include ".\IMusic.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class SDLMusic */
/** SDL Music resource */
class SDLMusic
	: public IMusic
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	
	/// Constructor
	/// \param pResource - raw data that contains music data
	/// \param pMusic - Loaded SDL music resource
	SDLMusic(IResource* pResource, Mix_Music* pMusic);

	/** Default Destructor */
	~SDLMusic();

	/// Get Mix_Music* resource
	/// \return pointer to a Mix_Music 
	virtual void* GetNativeMusic() override final;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	Mix_Music* m_pMusic;	///< Actual SDL music resource

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}