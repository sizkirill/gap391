#include "ISound.h"

using yang::ISound;

ISound::ISound(IResource* pResource)
    :IResource(std::move(*pResource))
{
	
}

ISound::~ISound()
{
	
}