#include "IMusic.h"

using yang::IMusic;

IMusic::IMusic(IResource* pResource)
    :IResource(std::move(*pResource))
{
	
}

IMusic::~IMusic()
{
	
}