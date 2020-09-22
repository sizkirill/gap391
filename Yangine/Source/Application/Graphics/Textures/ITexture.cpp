#include "ITexture.h"

using yang::ITexture;

ITexture::ITexture(IResource* pResource)
    :IResource(std::move(*pResource))
{
	
}

ITexture::~ITexture()
{
	
}

yang::ITexture::ITexture()
	:IResource()
{
}
