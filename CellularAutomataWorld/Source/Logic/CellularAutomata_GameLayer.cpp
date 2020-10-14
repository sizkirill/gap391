#include "CellularAutomata_GameLayer.h"
#include <Utils/Logger.h>
#include <Views/CameraView.h>

bool CellularAutomata_GameLayer::Init(const yang::ApplicationLayer& app)
{
    LOG_CATEGORY("Mod05", 0, Green, Dark);

    if (!yang::IGameLayer::Init(app))
    {
        LOG(Error, "Failed to initialize GameLayer");
        return false;
    }

    m_map.Init("Assets/Maps/World.xml");

    auto pCamera = SpawnActor("Assets/Actors/Camera.xml");

    if (!pCamera)
    {
        return false;
    }

    auto pCameraView = std::make_unique<CameraView>();

    if (!pCameraView)
    {
        return false;
    }

    pCameraView->SetControlledActor(pCamera);
    pCameraView->Init(app);
    pCameraView->SetMap(&m_map);
    AddView(std::move(pCameraView));


    return true;
}

void CellularAutomata_GameLayer::Update(float deltaSeconds)
{
    yang::IGameLayer::Update(deltaSeconds);
    m_map.UpdateFrame(deltaSeconds);
}
