#include "IAudio.h"

#include "SDLAudio.h"

using yang::IAudio;

IAudio::IAudio()
{
	
}

IAudio::~IAudio()
{
	
}

std::unique_ptr<IAudio> yang::IAudio::Create()
{
	return std::make_unique<SDLAudio>();
}
