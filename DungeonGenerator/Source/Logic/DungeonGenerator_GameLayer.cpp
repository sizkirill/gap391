#include "DungeonGenerator_GameLayer.h"
#include <Utils/Logger.h>

#include <Logic/System/CameraSystem.h>
#include <Logic/Components/CameraComponent.h>
#include <Views/CameraView.h>

bool DungeonGenerator_GameLayer::Init(const yang::ApplicationLayer& app)
{
    LOG_CATEGORY("DungeonGenerator", 0, Green, Dark);

    m_pCameraSystem = std::make_shared<CameraSystem>(app);
    m_actorFactory.RegisterComponent<CameraComponent>(std::weak_ptr<CameraSystem>(m_pCameraSystem));

    if (!yang::IGameLayer::Init(app))
    {
        LOG(Error, "Failed to initialize GameLayer");
        return false;
    }

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

void DungeonGenerator_GameLayer::Update(float deltaSeconds)
{
    yang::IGameLayer::Update(deltaSeconds);
    m_pCameraSystem->Update(deltaSeconds);
}
