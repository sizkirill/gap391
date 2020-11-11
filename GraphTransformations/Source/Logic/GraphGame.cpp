#include "GraphGame.h"
#include <Utils/Logger.h>
#include <Views/CameraView.h>

bool GraphGame::Init(const yang::ApplicationLayer& app)
{
    LOG_CATEGORY(Graphs, 0, Green, Dark);

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
    AddView(std::move(pCameraView));

    return true;
}
