#pragma once
/** \file ResourceCache.h */
/** Resource storing singleton class */
#include <unordered_map>
#include <string>
#include <memory>
#include <type_traits>
#include <cassert>

#include <Application/Graphics/IGraphics.h>
#include <Application/Graphics/Fonts/IFontLoader.h>
#include <Application/Audio/IAudio.h>

#include <Application/Audio/Music/IMusic.h>
#include <Application/Audio/Sound/ISound.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Application/Graphics/Fonts/IFont.h>

#include "Resource.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
    class ApplicationLayer;
/** \class ResourceCache */
/** Resource storing singleton class */
class ResourceCache
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	ResourceCache();

	/** Default Destructor */
	~ResourceCache();

    /// Loads the resource, or finds it in the lookup table if it's already loaded.
    /// \tparam ResourceType - ResourceType to load. Must be child of IResource
    /// \param filepath - path to the resource file
    /// \param args - additional arguments needed to load and identify the resource. Example: font size for the Font resource
    /// \return shared pointer to the resource
    template <class ResourceType, class... Args>
    std::shared_ptr<ResourceType> Load(const char* filepath, Args&&... args);

    /// Initializes the resource cache
    /// \param app - the application layer
    /// \return true if initialized successfully
    bool Init(const ApplicationLayer& app);

    /// Deallocates all loaded resources
	void Cleanup();

    /// Singleton getter
    static ResourceCache* Get();

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    std::unordered_map<std::string, std::shared_ptr<IResource>> m_resourceMap;  ///< Resource lookup table

    IGraphics* m_pGraphics;                                                     ///< Loads images
    IAudio* m_pAudio;                                                           ///< Loads audio   
    IFontLoader* m_pFontLoader;                                                 ///< Loads fonts


	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    /// Internal function to read file to a vector of bytes
    /// \param filepath - path to the resource
    /// \return Shared pointer to a raw resource
    std::shared_ptr<IResource> LoadResource(const char* filepath);

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};

template<class ResourceType, class... Args>
inline std::shared_ptr<ResourceType> ResourceCache::Load(const char* filepath, Args&&... args)
{
    static_assert(std::is_base_of_v<IResource, ResourceType>, "ResourceType template parameter must be a child of IResource");
	assert(filepath && "filepath should be valid string");

    if (m_resourceMap.count(filepath))
    {
        return std::static_pointer_cast<ResourceType>(m_resourceMap[filepath]);
    }

    std::shared_ptr<IResource> pLoaded = LoadResource(filepath);

    if constexpr (std::is_same_v<ResourceType, ITexture>)
    {
        std::shared_ptr<ITexture> pTexture = m_pGraphics->LoadTexture(pLoaded.get());
        m_resourceMap[pTexture->GetName()] = pTexture;
        return pTexture;
    }
    else if constexpr (std::is_same_v<ResourceType, ISound>)
    {
        std::shared_ptr<ISound> pSound = m_pAudio->LoadSound(pLoaded.get());
        m_resourceMap[pSound->GetName()] = pSound;
        return pSound;
    }
    else if constexpr (std::is_same_v<ResourceType, IMusic>)
    {
        std::shared_ptr<IMusic> pMusic = m_pAudio->LoadMusic(pLoaded.get());
        m_resourceMap[pMusic->GetName()] = pMusic;
        return pMusic;
    }
    else if constexpr (std::is_same_v<ResourceType, IFont>)
    {
        std::shared_ptr<IFont> pFont = m_pFontLoader->LoadFont(pLoaded.get(), std::forward<Args>(args)...);
		if (pFont)
			m_resourceMap[pFont->GetName()] = pFont;
        return pFont;
    }

	// Have to construct the string here to avoid compiler warning. Doesn't matter, because the string will be constructed anyway in the [] map operator
	std::string path(filepath);
    m_resourceMap[path] = pLoaded;
    return std::static_pointer_cast<ResourceType>(pLoaded);
}

}