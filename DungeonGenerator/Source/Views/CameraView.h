#pragma once

#include <Views/IView.h>
#include <Utils/Vector2.h>

namespace yang
{
    class IGraphics;
    class IKeyboard;
    class IMouse;
    class IAudio;
    class IGameLayer;
    class IEvent;
}

class DungeonMap;
class GraphSearcher;

class CameraView : public yang::IView
{
public:
    CameraView();

    ~CameraView();

    virtual bool Init(const yang::ApplicationLayer& app) override final;
    virtual void UpdateInput() override final;
    virtual void ViewScene() override final;
    void SetMap(DungeonMap* pMap);
private:
    yang::IGraphics* m_pGraphics;
    yang::IKeyboard* m_pKeyboard;
    yang::IMouse* m_pMouse;
    yang::IAudio* m_pAudio;
    yang::IGameLayer* m_pGameLayer;

    DungeonMap* m_pMap;
};