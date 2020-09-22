#include "ResourceCache.h"
#include <cctype> // std::tolower
#include <algorithm>
#include <fstream>

#include <Utils/Logger.h>
#include <Application/ApplicationLayer.h>

using yang::ResourceCache;

ResourceCache::ResourceCache()
    :m_pAudio(nullptr)
    ,m_pFontLoader(nullptr)
    ,m_pGraphics(nullptr)
{
    
}

ResourceCache::~ResourceCache()
{

}

std::shared_ptr<yang::IResource> yang::ResourceCache::LoadResource(const char* filepath)
{
    // Sanitizing the filepath
    std::string path = filepath;
    std::transform(path.begin(), path.end(), path.begin(), std::tolower);
    std::replace(path.begin(), path.end(), '\\', '/');

    // Load the resource
    // Find the size of the file
    std::ifstream inFile(path.c_str(), std::ios::in | std::ios_base::binary);

    if (inFile.fail())
    {
        LOG(Error, "Could not load resource: %s", filepath);
        return nullptr;
    }

    inFile.seekg(0, std::ios::end);
    size_t length = static_cast<size_t>(inFile.tellg());
    inFile.seekg(0, std::ios::beg);
    std::vector<std::byte> data(length);
    inFile.read(reinterpret_cast<char*>(data.data()), length);
    inFile.close();

    return std::make_shared<IResource>(path, std::move(data));
}

bool yang::ResourceCache::Init(const ApplicationLayer& app)
{
    m_pAudio = app.GetAudio();
    m_pFontLoader = app.GetFontLoader();
    m_pGraphics = app.GetGraphics();

    return true;
}

void yang::ResourceCache::Cleanup()
{
	m_resourceMap.clear();
}

ResourceCache* yang::ResourceCache::Get()
{
    static ResourceCache instance;
    return &instance;
}
