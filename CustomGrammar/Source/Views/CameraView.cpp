#include "CameraView.h"
#include <Application/ApplicationLayer.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Event/Input/MouseButtonEvent.h>
#include <Utils/Random.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Event/Input/KeyboardInputEvent.h>

CameraView::CameraView()
    :m_pGraphics(nullptr)
    ,m_pKeyboard(nullptr)
    ,m_pMouse(nullptr)
    ,m_pAudio(nullptr)
    ,m_pGameLayer(nullptr)
    ,m_pSystem(nullptr)
{
}

CameraView::~CameraView()
{
    yang::EventDispatcher::Get()->RemoveEventListener(yang::KeyboardInputEvent::kEventId, m_eventListenerIndex);
}

bool CameraView::Init(const yang::ApplicationLayer& app)
{
    m_pGraphics = app.GetGraphics();
    m_pKeyboard = app.GetKeyboard();
    m_pMouse = app.GetMouse();
    m_pAudio = app.GetAudio();
    m_pGameLayer = app.GetGameLayer();

    m_pSystem = std::make_unique<RegularGrammar>(m_pGraphics);
    m_pSystem->Init("Assets/Grammars/RoadMap.xml");
    m_pSystem->RunGrammar();
    m_pSystem->BuildLines(m_pGraphics);

    m_eventListenerIndex = yang::EventDispatcher::Get()->AddEventListener(yang::KeyboardInputEvent::kEventId, [this](yang::IEvent* pEvent) { HandleInputEvent(pEvent); });

    return true;
}

void CameraView::UpdateInput()
{
}

void CameraView::ViewScene()
{
    m_pGraphics->StartDrawing(0, 0, 0, 255);

    m_pSystem->Render(m_pGraphics);

    auto& actors = m_pGameLayer->GetActors();
    for (auto& actorPair : actors)
    {
        actorPair.second->Render(m_pGraphics);
    }

    m_pGraphics->EndDrawing();
}

void CameraView::HandleInputEvent(yang::IEvent* pEvent)
{
    if (pEvent->GetEventId() == yang::KeyboardInputEvent::kEventId)
    {
        auto pKeyboardEvent = static_cast<yang::KeyboardInputEvent*>(pEvent);
        if (pKeyboardEvent->GetEventType() == yang::KeyboardInputEvent::EventType::kKeyReleased)
        {
            if (pKeyboardEvent->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kSPACE)
            {
                m_pSystem->Reset();
                m_pSystem->RunGrammar();
                m_pSystem->BuildLines(m_pGraphics);
            }
        }
    }
}

