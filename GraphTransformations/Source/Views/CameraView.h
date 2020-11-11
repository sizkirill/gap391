#pragma once

#include <Views/IView.h>
#include <Utils/Vector2.h>
#include <Logic/Graph/Graph.h>
#include <Logic/Graph/GraphDrawer.h>
#include <Logic/Graph/RuleContainer.h>
#include <Logic/Graph/GraphPrinter.h>
#include <Utils/Random.h>
#include <memory>
#include <iostream>

namespace yang
{
    class IGraphics;
    class IKeyboard;
    class IMouse;
    class IAudio;
    class IGameLayer;
    class IEvent;
    class ITexture;
}

class CameraView : public yang::IView
{
public:
    CameraView();

    ~CameraView();

    virtual bool Init(const yang::ApplicationLayer& app) override final;
    virtual void UpdateInput() override final;
    virtual void ViewScene() override final;
private:
    yang::IGraphics* m_pGraphics;
    yang::IKeyboard* m_pKeyboard;
    yang::IMouse* m_pMouse;
    yang::IAudio* m_pAudio;
    yang::IGameLayer* m_pGameLayer;

    RuleContainer m_ruleContainer;
    GraphPrinter<std::ostream&> m_graphPrinter;
    Graph m_graph;
    GraphDrawer m_graphDrawer;
    yang::XorshiftRNG m_rngDevice;

    size_t m_eventListenerIndex;

    void HandleInputEvent(yang::IEvent* pEvent);
};