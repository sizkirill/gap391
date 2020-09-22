#include "DungeonGenerator_AppLayer.h"

#include <Logic/DungeonGenerator_GameLayer.h>

std::unique_ptr<yang::IGameLayer> DungeonGenerator_AppLayer::CreateGameLayer()
{
	return std::make_unique<DungeonGenerator_GameLayer>();
}
