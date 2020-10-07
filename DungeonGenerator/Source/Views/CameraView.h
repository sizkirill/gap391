#pragma once

#include <Views/IView.h>
#include <Utils/Vector2.h>
#include <memory>

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

class DungeonMap;
class GraphSearcher;
class WorldMap;

class CameraView : public yang::IView
{
public:
    CameraView();

    ~CameraView();

    virtual bool Init(const yang::ApplicationLayer& app) override final;
    virtual void UpdateInput() override final;
    virtual void ViewScene() override final;
    void SetMap(WorldMap* pMap);
private:
    yang::IGraphics* m_pGraphics;
    yang::IKeyboard* m_pKeyboard;
    yang::IMouse* m_pMouse;
    yang::IAudio* m_pAudio;
    yang::IGameLayer* m_pGameLayer;

    WorldMap* m_pMap;
};