#include "ProcessManager.h"
#include <Utils/Logger.h>
#include <Logic/Actor/Actor.h>

using yang::ProcessManager;

ProcessManager::ProcessManager()
{
	
}

ProcessManager::~ProcessManager()
{
    AbortAllProcesses();
}

void yang::ProcessManager::UpdateProcesses(float deltaSeconds)
{

	///////////////////////////////////////////////////////////////////////////
	//																		 //
    // If the order of processes ever matters, revert the changes below.	 //
	//																		 //
	///////////////////////////////////////////////////////////////////////////

    // LOG_ONCE(TODO, PopNSwap, "If this ever slows down due to vector::erase, use PopNSwap instead of erase, but that will destroy the order of proccesses. Does it matter?");

    //size_t index = 0;
    //while (index != m_pProcesses.size())
    //{
    //    auto pProcess = m_pProcesses[index];
    //    IProcess::State state = pProcess->GetState();

    //    if (state == IProcess::State::kRunning)
    //    {
    //        pProcess->Update(deltaSeconds);
    //    }
    //    else if (state == IProcess::State::kUninitialized)
    //    {
    //        if (pProcess->Init())
    //        {
    //            pProcess->Resume();
    //        }
    //        else
    //        {
    //            // Remove the process!
    //            m_pProcesses.erase(m_pProcesses.cbegin() + index);
    //            continue;
    //        }
    //    }
    //    else if (pProcess->IsDead())
    //    {
    //        if (state == IProcess::State::kSucceeded)
    //        {
    //            pProcess->OnSucceed();
    //            auto pChild = pProcess->RemoveChild();
    //            if (pChild)
    //            {
    //                AttachProcess(pChild);
    //            }
    //        }
    //        else if (state == IProcess::State::kAborted)
    //        {
    //            pProcess->OnAbort();
    //        }
    //        else if (state == IProcess::State::kFailed)
    //        {
    //            pProcess->OnFail();
    //        }

    //        m_pProcesses.erase(m_pProcesses.cbegin() + index);
    //        
    //        continue;
    //    }
    //    
    //    // Only increment the index if a process was not removed from the vector
    //    ++index;
    //}

    for (size_t i = 0; i < m_pProcesses.size(); ++i)
    {
        auto& pProcess = m_pProcesses[i];
        IProcess::State state = pProcess->GetState();
        std::pair<IProcess*, size_t> processIndexPair = { nullptr, 0 };

        switch (state)
        {
        case IProcess::State::kRunning:
        {
            pProcess->Update(deltaSeconds);
            break;
        }
        case IProcess::State::kUninitialized:
        {
            if (pProcess->Init())
            {
                pProcess->Resume();
            }
            else
            {
                processIndexPair.first = pProcess.get();
                processIndexPair.second = i;
            }
            break;
        }
        case IProcess::State::kSucceeded:
        {
            pProcess->OnSucceed();
            auto pChild = pProcess->RemoveChild();
            if (pChild)
            {
                AttachProcess(pChild);
            }
            processIndexPair.first = pProcess.get();
            processIndexPair.second = i;
            break;
        }
        case IProcess::State::kAborted:
        {
            pProcess->OnAbort();
            processIndexPair.first = pProcess.get();
            processIndexPair.second = i;
            break;
        }
        case IProcess::State::kFailed:
        {
            pProcess->OnFail();
            processIndexPair.first = pProcess.get();
            processIndexPair.second = i;
            break;
        }
        default:
        {
            LOG(Warning, "Reached default");
            break;
        }
        }
        if (processIndexPair.first)
        {
            RemoveProcess(processIndexPair.second);
            --i;
        }
    }
}

void yang::ProcessManager::AttachProcess(std::shared_ptr<IProcess> pProcess)
{
    m_pProcesses.emplace_back(pProcess);
}

void yang::ProcessManager::AbortProcessesOnActor(Id actorId)
{
    LOG_ONCE(TODO, TimeComplexity, "Think how to make it O(1)");
    for (size_t i = 0; i < m_pProcesses.size(); ++i)
    {
        Actor* pOwner = m_pProcesses[i]->GetOwner();
        if (pOwner && pOwner->GetId() == actorId)
        {
            RemoveProcess(i);
            --i;
        }
    }
}

void yang::ProcessManager::AbortAllProcesses()
{
    for (auto& pProcess : m_pProcesses)
    {
        if (pProcess->IsAlive())
        {
            pProcess->Abort();
            pProcess->OnAbort();
        }
    }
    m_pProcesses.clear();
}

void yang::ProcessManager::RemoveProcess(size_t index)
{
    std::swap(m_pProcesses[index], m_pProcesses[m_pProcesses.size() - 1]);
    m_pProcesses.pop_back();
}
