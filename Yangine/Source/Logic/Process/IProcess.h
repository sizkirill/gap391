#pragma once
#include <Utils/Logger.h>
/** \file IProcess.h */
/** Process interface description */
#include <functional>
#include <memory>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class Actor;
/** \class IProcess */
/** Base class for all processes */
class IProcess
{
public:

    /// \enum State
    /// All possible states of a process
    enum class State
    {
        kUninitialized, ///< Process has not been initialized yet
        kRunning,       ///< Process is currently running
        kPaused,        ///< Process is paused
        kSucceeded,     ///< Process has successfully finished
        kFailed,        ///< Process failed during execution
        kAborted        ///< Process was aborted before completion
    };

	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param pOwner - actor that owns this process
	IProcess(Actor* pOwner);

	/** Default Destructor */
	virtual ~IProcess();

    /// Initializes the process
    /// Component linking should happen here
    /// \return true if successfully initialized
    virtual bool Init();

    /// Updates the process by deltaSeconds
    /// \param deltaSeconds - amount of seconds passed since last frame
    virtual void Update(float deltaSeconds) = 0;

    /// Called when process aborts. Effectively calls the abort callback
    void OnAbort();

    /// Called when process successfully finished. Effectively calls the succeed callback
    void OnSucceed();

    /// Called when process fails to finish. Effectively calls the fail callback
    void OnFail();

    /// Remove the child process from this process
    /// \return shared pointer to the removed process
    std::shared_ptr<IProcess> RemoveChild();

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    State m_state;                              ///< Current state of the process
    std::function<void()> m_succeedCallback;    ///< Callback that is called when process succeeds. Can be null
    std::function<void()> m_abortCallback;      ///< Callback that is called when process is aborted. Can be null
    std::function<void()> m_failCallback;       ///< Callback that is called when process fails. Can be null
    std::shared_ptr<IProcess> m_pChild;         ///< Child process, that is executed if this process is successful. Can be null

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

protected:
    // --------------------------------------------------------------------- //
    // Protected Member Variables
    // --------------------------------------------------------------------- //
    Actor* m_pOwner;                            ///< Actor that owns this process

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
    void Succeed()  { m_state = State::kSucceeded; }    ///< Set current process state to Succeeded \see yang::IProcess::State
    void Fail()     { m_state = State::kFailed; }       ///< Set current process state to Failed \see yang::IProcess::State
    void Abort()    { m_state = State::kAborted; }      ///< Set current process state to Aborted \see yang::IProcess::State
    void Pause()    { m_state = State::kPaused; }       ///< Set current process state to Paused \see yang::IProcess::State
    void Resume()   { m_state = State::kRunning; }      ///< Set current process state to Running \see yang::IProcess::State

    State GetState()    const { return m_state; }       ///< Get current process state

    /// \return true if the process is alive
    bool IsAlive()      const { return (m_state == State::kRunning || m_state == State::kPaused); }
    /// \return true if the process is dead
    bool IsDead()       const { return (m_state == State::kFailed || m_state == State::kAborted || m_state == State::kSucceeded); }

    /// Set the callback for succeed
    /// \param callback - new succeed callback
    void SetOnSucceed(std::function<void()> callback) { m_succeedCallback = callback; }

    /// Set the callback for abort
    /// \param callback - new abort callback
    void SetOnAbort(std::function<void()> callback) { m_abortCallback = callback; }

    /// Set the callback for fail
    /// \param callback - new fail callback
    void SetOnFail (std::function<void()> callback) { m_failCallback = callback; }

    /// Attaches the child process
    /// \param pProcess - child process to attach
    void AttachChild(std::shared_ptr<IProcess> pProcess) { m_pChild = pProcess; }

    /// Get the actor that owns this process
    /// \return Actor that owns this process
    Actor* GetOwner() const { return m_pOwner; }
};
}