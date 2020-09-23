#include "DungeonGenerator_GameLayer.h"
#include <Utils/Logger.h>

#include <Logic/Map/MapGenerator.h>
#include <Views/CameraView.h>

bool DungeonGenerator_GameLayer::Init(const yang::ApplicationLayer& app)
{
    LOG_CATEGORY("DungeonGenerator", 0, Green, Dark);

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

    //MapGenerator mapGen{ 42424242 }; //< some seed for debug. Put nothing to have a random seed
    MapGenerator mapGen; //< some seed for debug. Put nothing to have a random seed

    mapGen.Init("Assets/Maps/MapGen.xml");

    m_map = mapGen.GenerateMap();
    m_map.PlaceDoors();
    m_map.GenerateRoomGraph();

    pCameraView->SetControlledActor(pCamera);
    pCameraView->Init(app);
    pCameraView->SetMap(&m_map);
    AddView(std::move(pCameraView));

    return true;
}
