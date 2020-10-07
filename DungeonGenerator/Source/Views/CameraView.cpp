#include "CameraView.h"
#include <Application/ApplicationLayer.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Event/Input/MouseButtonEvent.h>
#include <Logic/Map/WorldMap.h>

#include <Utils/Random.h>

CameraView::CameraView()
    :m_pGraphics(nullptr)
    ,m_pKeyboard(nullptr)
    ,m_pMouse(nullptr)
    ,m_pAudio(nullptr)
    ,m_pGameLayer(nullptr)
    ,m_pMap(nullptr)
{
}

CameraView::~CameraView()
{
}

bool CameraView::Init(const yang::ApplicationLayer& app)
{
    m_pGraphics = app.GetGraphics();
    m_pKeyboard = app.GetKeyboard();
    m_pMouse = app.GetMouse();
    m_pAudio = app.GetAudio();
    m_pGameLayer = app.GetGameLayer();

    return true;
}

void CameraView::UpdateInput()
{
}

void CameraView::ViewScene()
{
    m_pGraphics->StartDrawing(0, 0, 0, 255);

    m_pMap->Render(m_pGraphics);

    auto& actors = m_pGameLayer->GetActors();
    for (auto& actorPair : actors)
    {
        actorPair.second->Render(m_pGraphics);
    }

    m_pGraphics->EndDrawing();
}

void CameraView::SetMap(WorldMap* pMap)
{
    m_pMap = pMap;
    m_pMap->Init("Assets/Maps/World.xml");
    m_pMap->Generate(0x1298736);
}

