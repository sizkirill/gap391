#include "IFont.h"

using yang::IFont;

IFont::IFont(IResource* pResource)
    :IResource(std::move(*pResource))
{
}

IFont::~IFont()
{
	
}