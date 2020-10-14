#include "CellularAutomata_AppLayer.h"

#include <Logic/CellularAutomata_GameLayer.h>

std::unique_ptr<yang::IGameLayer> CellularAutomata_AppLayer::CreateGameLayer()
{
	return std::make_unique<CellularAutomata_GameLayer>();
}
