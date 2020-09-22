#include "CreateActorEvent.h"

using yang::CreateActorEvent;

CreateActorEvent::CreateActorEvent(const char* xmlFilepath, std::optional<yang::FVec2> position, bool isEnemy)
    :m_xmlFilepath(xmlFilepath)
    ,m_positionToSpawn(position)
    ,m_isEnemy(isEnemy)
{
	
}

CreateActorEvent::~CreateActorEvent()
{
	
}