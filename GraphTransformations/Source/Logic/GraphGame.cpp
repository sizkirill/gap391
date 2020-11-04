#include "GraphGame.h"
#include <Utils/Logger.h>

bool GraphGame::Init(const yang::ApplicationLayer& app)
{
    LOG_CATEGORY("Mod 08 - GraphTransformations", 0, Green, Dark);

    if (!yang::IGameLayer::Init(app))
    {
        LOG(Error, "Failed to initialize GameLayer");
        return false;
    }

    return true;
}
