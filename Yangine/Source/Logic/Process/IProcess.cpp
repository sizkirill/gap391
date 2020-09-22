#include "IProcess.h"

using yang::IProcess;

IProcess::IProcess(yang::Actor* pOwner)
    :m_state(State::kUninitialized)
    ,m_pOwner(pOwner)
{
	
}

IProcess::~IProcess()
{
	
}

bool yang::IProcess::Init()
{
    LOG_ONCE(TODO, ProcessInit, "Change Init default implementation at IProcess base class"); 
    return true;
}

void yang::IProcess::OnAbort()
{
    if (m_abortCallback)
    {
        m_abortCallback();
    }
}

void yang::IProcess::OnSucceed()
{
    if (m_succeedCallback)
    {
        m_succeedCallback();
    }
}

void yang::IProcess::OnFail()
{
    if (m_failCallback)
    {
        m_failCallback();
    }
}

std::shared_ptr<IProcess> yang::IProcess::RemoveChild()
{
    auto pChild = m_pChild;
    m_pChild = nullptr;
    return pChild;
}
