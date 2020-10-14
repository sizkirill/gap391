#include "PerlinNoiseWorld_AppLayer.h"

#include <Logic/PerlinNoiseWorld_GameLayer.h>

std::unique_ptr<yang::IGameLayer> PerlinNoiseWorld_AppLayer::CreateGameLayer()
{
	return std::make_unique<PerlinNoiseWorld_GameLayer>();
}
