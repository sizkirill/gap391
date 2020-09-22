#include "SDLAudio.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <Application/Resources/Resource.h>
#include <Application/Audio/Music/SDLMusic.h>
#include <Application/Audio/Sound/SDLSound.h>

#include <Utils/Logger.h>

using yang::SDLAudio;

SDLAudio::SDLAudio()
	: m_initialized(false)
{
	
}

SDLAudio::~SDLAudio()
{
	if (m_initialized)
	{
		Mix_CloseAudio();
		Mix_Quit();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	}
}

bool yang::SDLAudio::Init()
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO))
	{
		LOG(Error, "Failed to initialize SDL Audio: %s", Mix_GetError());
		return false;
	}

	m_initialized = true;

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024))
	{
		LOG(Error, "Failed to open SDL_mixer audio: %s", Mix_GetError());
		return false;
	}

	int mixFlags = MIX_INIT_MP3 | MIX_INIT_OGG;
	if (Mix_Init(mixFlags) != mixFlags)
	{
		LOG(Error, "Failed to initialize SDL_mixer: %s", Mix_GetError());
		return false;
	}

	return true;
}

bool yang::SDLAudio::PlayMusic(IMusic* pMusic, i8 volume, i8 loops)
{
	if (!pMusic)
	{
		LOG(Error, "pMusic == nullptr in %s", __FUNCTION__);
		return false;
	}

    if (volume != -1)
    {
        Mix_VolumeMusic(volume);
    }

	if (Mix_PlayMusic(reinterpret_cast<Mix_Music*>(pMusic->GetNativeMusic()), loops))
	{
		LOG(Error, "Failed to play music. Error: %s", Mix_GetError());
		return false;
	}

	return true;
}

bool yang::SDLAudio::PlaySound(ISound* pSound, i8 volume)
{
	if (!pSound)
	{
		LOG(Error, "pSound == nullptr in %s", __FUNCTION__);
		return false;
	}

	int channelNum = Mix_PlayChannel(-1, reinterpret_cast<Mix_Chunk*>(pSound->GetNativeSound()), 0);
    if (volume != -1)
    {
        Mix_Volume(channelNum, volume);
    }
	return true;
}

bool yang::SDLAudio::FadeInMusic(IMusic* pMusic, ui32 ms, i8 volume, i8 loops)
{
    if (pMusic)
    {
		LOG(Error, "pMusic == nullptr in %s", __FUNCTION__);
		return false;
    }

    if (volume != -1)
    {
        Mix_VolumeMusic(volume);
    }

    if (Mix_FadeInMusic(reinterpret_cast<Mix_Music*>(pMusic->GetNativeMusic()), loops, ms))
    {
        LOG(Error, "Failed to play music. Error: %s", Mix_GetError());
        return false;
    }

    return true;
}

void yang::SDLAudio::FadeOutMusic(ui32 ms)
{
    Mix_FadeOutMusic(ms);
}

void yang::SDLAudio::StopMusic()
{
    Mix_HaltMusic();
}

void yang::SDLAudio::PauseMusic()
{
    if (Mix_PlayingMusic())
    {
        Mix_PauseMusic();
    }
}

void yang::SDLAudio::ResumeMusic()
{
    // Checking if music was paused and not halted
    if (Mix_PausedMusic() && Mix_PlayingMusic())
    {
        Mix_ResumeMusic();
    }
}

void yang::SDLAudio::SetMusicVolume(i8 volume)
{
    Mix_VolumeMusic(volume);
}

std::shared_ptr<yang::ISound> yang::SDLAudio::LoadSound(IResource* pResource)
{
	SDL_RWops* pOps = SDL_RWFromConstMem(pResource->GetData().data(), static_cast<int>(pResource->GetData().size()));
	Mix_Chunk* pSound = Mix_LoadWAV_RW(pOps, 0);

	if (!pSound)
	{
		LOG(Error, "Unable to load sound. Error: %s", SDL_GetError());
		return nullptr;
	}

	return std::make_shared<SDLSound>(pResource, pSound);
}

std::shared_ptr<yang::IMusic> yang::SDLAudio::LoadMusic(IResource* pResource)
{
	SDL_RWops* pOps = SDL_RWFromConstMem(pResource->GetData().data(), static_cast<int>(pResource->GetData().size()));
	Mix_Music* pMusic = Mix_LoadMUS_RW(pOps, 0);

	if (!pMusic)
	{
		LOG(Error, "Unable to load music. Error: %s", SDL_GetError());
		return nullptr;
	}

	return std::make_shared<SDLMusic>(pResource, pMusic);
}
