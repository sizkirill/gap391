#pragma once
/** \file SDLAudio.h */
/** Specialization of IAudio interface for SDL_mixer library */

#include ".\IAudio.h"

#include <unordered_map>
#include <string>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class SDLAudio */
/** Wrapper for SDL_mixer audio library */
class SDLAudio final
	: public IAudio
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	SDLAudio();

	/** Default Destructor */
	~SDLAudio();

	/// Initializes the SDL Audio system
	/// \return true if SDL_mixer library was initialized successfully
	virtual bool Init() override final;

	/// Plays music
	/// \param pMusic - pointer to SDL Music resource to be played
	/// \param volume - int in range -1 - 127. -1 is the default value passed in the function, it doesn't change the volume
	/// \param loops - int in range -1 - 127. -1 is the default value passed in the function and means the music will play forever. 0 plays the music zero times...
	/// \return true if the music was successfully started
	virtual bool PlayMusic(IMusic* pMusic, i8 volume = -1, i8 loops = -1) override final;

	/// Plays sound
	/// \param pSound - pointer to SDL Sound resource to be played
	/// \param volume - int in range -1 - 127. -1 is the default value passed in the function, it doesn't change the volume
	/// \return true if the sound was successfully played
	virtual bool PlaySound(ISound* pSound, i8 volume = -1) override final;

    // TODO: Do tests of the below functions

	/// Fades in music
	/// \param pMusic - pointer to SDL Music resource to be played
	/// \param ms - milliseconds of the fade in effect
	/// \param volume - int in range -1 - 127. -1 is the default value passed in the function, it doesn't change the volume
	/// \param loops - int in range -1 - 127. -1 is the default value passed in the function and means the music will play forever. 0 plays the music zero times...
	/// \return true if the music was successfully started
    virtual bool FadeInMusic(IMusic* pMusic, ui32 ms, i8 volume = -1, i8 loops = -1) override final;

	/// Fades out current music 
	/// \param ms - milliseconds of the fade out effect. Music will be stopped at the end of the fade out effect
    virtual void FadeOutMusic(ui32 ms) override final;

	/// Stops music
    virtual void StopMusic() override final;

	/// Pauses music (safe to call if there is no music playing)
    virtual void PauseMusic() override final;

	/// Resumes music if it was paused previously (safe to call if it was stopped though)
    virtual void ResumeMusic() override final;

	/// Sets the music volume
	/// \param volume - int in range 0-127 that represents the volume to set
    virtual void SetMusicVolume(i8 volume) override final;

	/// Loads sound from raw data resource
	/// \param pResource - Resource to load sound from
	/// \return shared ptr to SDL Sound resource
    virtual std::shared_ptr<ISound> LoadSound(IResource* pResource) override final;

	/// Loads music from raw data resource
	/// \param pResource - Resource to load music from
	/// \return shared ptr to Music resource
    virtual std::shared_ptr<IMusic> LoadMusic(IResource* pResource) override final;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	bool m_initialized;		///< Bool that says if audio system was initialized. For internal use, used for cleaning up properly.

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}