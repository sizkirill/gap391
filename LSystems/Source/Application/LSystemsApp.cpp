#include "LSystemsApp.h"

#include <Logic/LSystemsGame.h>

std::unique_ptr<yang::IGameLayer> LSystemsApp::CreateGameLayer()
{
	return std::make_unique<LSystemsGame>();
}
