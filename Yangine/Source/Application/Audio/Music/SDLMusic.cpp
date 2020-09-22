#include "SDLMusic.h"

using yang::SDLMusic;

SDLMusic::SDLMusic(IResource* pResource,  Mix_Music* pMusic)
    :IMusic(pResource)
	,m_pMusic(pMusic)
{
	
}

SDLMusic::~SDLMusic()
{
	Mix_FreeMusic(m_pMusic);
}

void* yang::SDLMusic::GetNativeMusic()
{
	return m_pMusic;
}
