#include "Resource.h"

using yang::IResource;

IResource::IResource(const std::string& filepath, std::vector<std::byte>&& data)
    :m_filepath(filepath)
    ,m_data(std::move(data))
{
	
}

IResource::~IResource()
{
	
}

yang::IResource::IResource()
{
}
