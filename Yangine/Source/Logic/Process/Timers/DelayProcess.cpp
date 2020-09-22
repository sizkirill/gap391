#include "DelayProcess.h"

using yang::DelayProcess;

yang::DelayProcess::DelayProcess(Actor* pOwner, float delay)
    :IProcess(pOwner)
    ,m_delay(delay)
{
}

DelayProcess::~DelayProcess()
{
	
}

void yang::DelayProcess::Update(float deltaSeconds)
{
    m_delay -= deltaSeconds;
    if (m_delay <= 0.f)
    {
        Succeed();
    }
}
