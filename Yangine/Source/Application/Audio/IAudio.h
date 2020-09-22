#pragma once
/** \file IAudio.h */
/** Audio system interface description */

#include <memory>
#include <Utils/Typedefs.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class ISound;
    class IMusic;
    class IResource;
/** \class IAudio */
/** Interface for sound system wrappers */
class IAudio
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	IAudio();

	/** Default Destructor */
	virtual ~IAudio();

    /// Initializes the Audio system
	/// \return true if Audio system was initialized successfully
	virtual bool Init() = 0;

    /// Plays music
    /// \param pMusic - pointer to Music resource to be played
    /// \param volume - int in range -1 - 127. -1 is the default value passed in the function, it doesn't change the volume
    /// \param loops - int in range -1 - 127. -1 is the default value passed in the function and means the music will play forever. 0 plays the music zero times...
	/// \return true if the music was successfully started
	virtual bool PlayMusic(IMusic* pMusic, i8 volume = -1, i8 loops = -1) = 0;

    /// Fades in music
    /// \param pMusic - pointer to Music resource to be played
    /// \param ms - milliseconds of the fade in effect
    /// \param volume - int in range -1 - 127. -1 is the default value passed in the function, it doesn't change the volume
    /// \param loops - int in range -1 - 127. -1 is the default value passed in the function and means the music will play forever. 0 plays the music zero times...
    /// \return true if the music was successfully started
    virtual bool FadeInMusic(IMusic* pMusic, ui32 ms, i8 volume = -1, i8 loops = -1) = 0;

    /// Stops music
    virtual void StopMusic() = 0;

    /// Fades out current music 
    /// \param ms - milliseconds of the fade out effect. Music will be stopped at the end of the fade out effect
    virtual void FadeOutMusic(ui32 ms) = 0;

    /// Pauses music (safe to call if there is no music playing)
    virtual void PauseMusic() = 0;

    /// Resumes music if it was paused previously (safe to call if it was stopped though)
    virtual void ResumeMusic() = 0;

    /// Plays sound
    /// \param pSound - pointer to a Sound resource to be played
    /// \param volume - int in range -1 - 127. -1 is the default value passed in the function, it doesn't change the volume
    /// \return true if the sound was successfully played
	virtual bool PlaySound(ISound* pSound, i8 volume = -1) = 0;

    /// Sets the music volume
    /// \param volume - int in range 0-127 that represents the volume to set
    virtual void SetMusicVolume(i8 volume) = 0;

    /// Creates the Audio System. 
	/// \return unique_ptr to Audio system
	static std::unique_ptr<IAudio> Create();

    /// Loads sound from raw data resource
    /// \param pResource - Resource to load sound from
    /// \return shared ptr to Sound resource
    virtual std::shared_ptr<ISound> LoadSound(IResource* pResource) = 0;

    /// Loads music from raw data resource
    /// \param pResource - Resource to load music from
    /// \return shared ptr to Music resource
    virtual std::shared_ptr<IMusic> LoadMusic(IResource* pResource) = 0;

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
}