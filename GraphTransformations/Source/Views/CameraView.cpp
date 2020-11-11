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
    ,m_ruleContainer("Assets/Graphs/Ruleset.xml")
    ,m_graphPrinter(std::cout)
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

    m_graph = Graph::BuildGraphFromXML("Assets/Graphs/Graph.xml");
    m_graphDrawer.BuildTileMap(m_graph);
    m_graphDrawer.Finalize();

    m_eventListenerIndex = yang::EventDispatcher::Get()->AddEventListener(yang::KeyboardInputEvent::kEventId, [this](yang::IEvent* pEvent) { HandleInputEvent(pEvent); });

    return true;
}

void CameraView::UpdateInput()
{
}

void CameraView::ViewScene()
{
    m_pGraphics->StartDrawing(0, 0, 0, 255);

    m_graphDrawer.Render(m_pGraphics);

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
            if (pKeyboardEvent->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::k1)
            {
                bool done = false;
                do
                {
                    // Get random rule based on weights
                    // The initial start->startingGraph rule has a huge weight, so it is most likely the first one to run, but random may think otherwise =/ as a hack I'll leave it like that
                    TransformationRule& rule = m_ruleContainer.GetWeightedRandomRule(m_rngDevice);

                    // get possible indices of a graph where we can apply the rule
                    auto possibleIndices = m_graph.FindNodeIndicesByType(rule.GetSourceNodeType());

                    // if there are none, continue to next iteration (there is always possible that entrance will expand into entrance->task, so the loop won't be infinite)
                    if (possibleIndices.size() > 0)
                    {
                        done = true;
                        auto index = possibleIndices[m_rngDevice.Rand(possibleIndices.size())];
                        m_graph.SpliceSubGraph(index, rule.GetResultGraph());
                        m_graphDrawer.Reset();
                        m_graphDrawer.BuildTileMap(m_graph);
                        m_graphDrawer.Finalize();
                        m_graphPrinter.PrintIteration(rule, m_graph);
                    } 
                } while (!done);
            }
        }
    }
}

