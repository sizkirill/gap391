#include "GameOfLife_AppLayer.h"

#include <Logic/GameOfLife_GameLayer.h>

std::unique_ptr<yang::IGameLayer> GameOfLife_AppLayer::CreateGameLayer()
{
	return std::make_unique<GameOfLife_GameLayer>();
}
