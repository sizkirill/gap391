#pragma once
/** \file DelayProcess.h */
/** Delay process description */

#include "..\IProcess.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class DelayProcess */
/** Executes Succeed callback after some amount of seconds */
class DelayProcess
	: public IProcess
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param pOwner - actor that owns this process
    /// \param delay - seconds of delay
	DelayProcess(Actor* pOwner, float delay);

	/** Default Destructor */
	virtual ~DelayProcess();

    /// Updates the process by deltaSeconds. Effectively subtracts deltaSeconds from delay
    /// \param deltaSeconds - amount of seconds passed since last frame
    virtual void Update(float deltaSeconds) override;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    float m_delay;  ///< amount of seconds to wait before executing succeed callback

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}