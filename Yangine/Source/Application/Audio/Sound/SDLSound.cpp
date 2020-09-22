#include "SDLSound.h"

using yang::SDLSound;

SDLSound::SDLSound(IResource* pResource, Mix_Chunk* pSound)
    :ISound(pResource)
	,m_pSound(pSound)
{
	
}

SDLSound::~SDLSound()
{
	Mix_FreeChunk(m_pSound);
}