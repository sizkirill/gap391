#include "ApplicationLayer.h"

#include <Utils/Math.h>
#include <Application/Resources/ResourceCache.h>

// TODO: remove after doing all implementation
#include <Application/Graphics/Fonts/SDLFontLoader.h>

#include <cassert>

using yang::ApplicationLayer;

ApplicationLayer::ApplicationLayer()
{
}

ApplicationLayer::~ApplicationLayer()
{
	
}

void yang::ApplicationLayer::Run()
{
	LOG(Boot, "Running...");
    using namespace std::chrono;
    time_point<steady_clock> last = steady_clock::now();

	//Main Loop!
	while (m_pWindow->ProcessEvents())
	{
        time_point<steady_clock> now = steady_clock::now();
        duration<float> deltaTime = now - last;
        float deltaSeconds = deltaTime.count();

        // Update Game (Input)
        m_pGameLayer->Update(deltaSeconds);
        m_pWindow->NextFrame();

        last = now;
	}
}

bool yang::ApplicationLayer::Init()
{
	LOG_CATEGORY(Info, 1, Green, Light);
	LOG_CATEGORY(Boot, 1, Cyan, Dark);
	LOG_CATEGORY(Error, 0, Red, Light);
    LOG_CATEGORY(Warning, 3, Yellow, Light);
    LOG_CATEGORY(TODO, 9, Magenta, Light);
	LOG_CATEGORY(Test, 3, White, Dark);

	m_pSystem = IOpSys::Create();

	if (!m_pSystem)
	{
		// Logger has not been set up yet.. cout the error
		return false;
	}

	Logger::Get()->Init(m_pSystem.get());

	m_pGameLayer = CreateGameLayer();

	if (!m_pGameLayer)
	{
		LOG(Error, "Failed to create game layer");
		Cleanup();
		return false;
	}
	LOG(Info, "Game: %s", m_pGameLayer->GetGameName());

	m_pWindow = m_pSystem->CreateSystemWindow(m_pGameLayer->GetGameName(), 1280, 720);

	m_pGraphics = IGraphics::Create();
	if (!m_pGraphics)
	{
		LOG(Error, "IGraphics::Create() failed.");
		Cleanup();
		return false;
	}

	if (!m_pGraphics->Initialize(m_pWindow.get()))
	{
		LOG(Error, "Graphics Initialize failed.");
		Cleanup();
		return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Init font loader after graphics. TODO: Actually implement it                       ///
    m_pFontLoader = std::make_unique<SDLFontLoader>(); // Change later to IFontLoader::Create()
	if (!m_pFontLoader->Init(m_pGraphics.get()))
	{
		LOG(Error, "Font Loader initialize failed.");
		Cleanup();
		return false;
	}
    /////////////////////////////////////////////////////////////////////////////////////////

    m_pAudio = IAudio::Create();

    if (!m_pAudio || !m_pAudio->Init())
    {
        LOG(Error, "Audio system failed to init");
        Cleanup();
        return false;
    }

    auto pKeyboard = IKeyboard::Create();
    
    if (!pKeyboard || !pKeyboard->Init())
    {
        LOG(Error, "Failed to init keyboard");
        Cleanup();
        return false;
    }
    
    auto pMouse = IMouse::Create();

    if (!pMouse || !pMouse->Init())
    {
        LOG(Error, "Failed to init mouse");
        Cleanup();
        return false;
    }

    m_pWindow->AttachKeyboard(std::move(pKeyboard));
    m_pWindow->AttachMouse(std::move(pMouse));

    m_pPhysics = IPhysicsSimulation::Create(FVec2(0.f, 9.8f));
    if (!m_pPhysics || !m_pPhysics->Init())
    {
        LOG(Error, "Failed to init physics");
        Cleanup();
        return false;
    }

	LOG(Boot, "ApplicationLayer::Init succeeded");

    // Init ResourceCache
    if (!ResourceCache::Get()->Init(*this))
    {
        LOG(Error, "ResourceCache failed to init");
        return false;
    }

    // Init game layer last
    if (!m_pGameLayer->Init(*this))
    {
        LOG(Error, "GameLayer failed to init");
        return false;
    }

	return true;
}

yang::IVec2 yang::ApplicationLayer::GetWindowDimensions() const
{
    assert(m_pWindow);
    return m_pWindow->GetDimensions();
}

void yang::ApplicationLayer::Cleanup()
{
	LOG(Boot, "Cleanup...");

    // TODO: when to cleanup?

	// Firstly cleanup resources
	m_pGameLayer->Cleanup();
    m_pGameLayer.reset();

	ResourceCache::Get()->Cleanup();

	// cleanup fonts before graphics
	m_pFontLoader.reset();
	m_pGraphics.reset();
    m_pWindow.reset();
    m_pPhysics.reset();
    m_pAudio.reset();
 

    Logger::Get()->Finish();

    m_pSystem.reset();
}
