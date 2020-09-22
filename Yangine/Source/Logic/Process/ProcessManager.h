#pragma once
#include <Logic/Process/IProcess.h>
#include <Utils/Typedefs.h>
#include <memory>
#include <vector>

/** \file ProcessManager.h */
/** ProcessManager class description */


//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class ProcessManager */
/** Class that is responsible for updating, attaching and destroying processes. Order of processes is not preserved when processes are removed */
class ProcessManager
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	ProcessManager();

	/** Default Destructor */
	~ProcessManager();

    /// Update all active processes
    /// \param deltaSeconds - amount of seconds passed since last frame
    void UpdateProcesses(float deltaSeconds);

    /// Adds a new process to the manager
    /// \param pProcess - process to add
    void AttachProcess(std::shared_ptr<IProcess> pProcess);

    /// Aborts all processes owned by an actor. Order of alive processes not preserved after call to this function.
    /// \param actorId - ID of the actor, whose processes should be aborted
    void AbortProcessesOnActor(Id actorId);

    /// Aborts all processes
    void AbortAllProcesses();
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    std::vector<std::shared_ptr<IProcess>> m_pProcesses;    ///< Vector of all processes

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    /// Private function to remove a process from the vector at the index position. Effectively swaps and pops the process -> Order is not preserved
    /// \param index - index of the process in the processes vector
    void RemoveProcess(size_t index);
public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}