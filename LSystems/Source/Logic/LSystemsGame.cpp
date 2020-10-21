#include "LSystemsGame.h"
#include <Utils/Logger.h>
#include <Utils/Random.h>
#include <Logic/Scripting/LuaManager.h>
#include <Views/CameraView.h>
#include <Logic/LSystems/LineBuilder.h>
#include <vector>

#include <Utils/Random.h>

bool LSystemsGame::Init(const yang::ApplicationLayer& app)
{
    LOG_CATEGORY(LSystems, 0, Green, Dark);

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
