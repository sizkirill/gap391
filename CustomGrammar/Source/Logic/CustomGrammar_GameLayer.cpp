#include "CustomGrammar_GameLayer.h"
#include <Utils/Logger.h>
#include <Logic/Grammar/LineBuilder.h>
#include <Views/CameraView.h>

bool CustomGrammar_GameLayer::Init(const yang::ApplicationLayer& app)
{
    LOG_CATEGORY(CustomGrammar, 0, Green, Dark);

    if (!yang::IGameLayer::Init(app))
    {
        LOG(Error, "Failed to initialize GameLayer");
        return false;
    }

    LineBuilder::ExposeToLua(m_luaManager);

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
