#include "CustomGrammar_AppLayer.h"

#include <Logic/CustomGrammar_GameLayer.h>

std::unique_ptr<yang::IGameLayer> CustomGrammar_AppLayer::CreateGameLayer()
{
	return std::make_unique<CustomGrammar_GameLayer>();
}
