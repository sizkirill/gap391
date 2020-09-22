#pragma once
/** \file IMusic.h */
/** Music interface description */

#include "..\..\Resources\Resource.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class IMusic */
/** Interface for a music resource */
class IMusic
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
	/// \param pResource - data to construct a music resource from
	IMusic(IResource* pResource);

	/** Default Destructor */
	virtual ~IMusic();

	/// Get underlying music resource
	/// \return music resource, depending on the underlying library
	virtual void* GetNativeMusic() = 0;

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