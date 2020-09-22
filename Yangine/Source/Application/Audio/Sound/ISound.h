#pragma once
/** \file ISound.h */
/** Sound interface description */

#include "..\..\Resources\Resource.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class ISound */
/** Interface for a sound resource */
class ISound
	: public IResource
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/// Constructor
    /// \param pResource - pointer to a raw resource data
	ISound(IResource* pResource);

	/// Default Destructor
	virtual ~ISound();

	/// Get the underlying sound resource
	/// \return pointer to a sound resource, depending on the underlying library
	virtual void* GetNativeSound() = 0;

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}