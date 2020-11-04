#include "GraphApp.h"

#include <Logic/GraphGame.h>

std::unique_ptr<yang::IGameLayer> GraphApp::CreateGameLayer()
{
	return std::make_unique<GraphGame>();
}
