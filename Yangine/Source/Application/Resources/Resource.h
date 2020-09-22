#pragma once
/** \file Resource.h */
/** Base interface for the data resources */

#include <vector>
#include <string>
#include <Utils/Typedefs.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class Resource */
/** Base interface for the data resources */
class IResource
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param filepath - path to the resource
    /// \param data - vector of raw bytes representing the data in the file
	IResource(const std::string& filepath, std::vector<std::byte>&& data);

    /// Explicitly defaulted move constructor
    IResource(IResource&&) = default;

    /// Explicitly defaulted move assignment operator
    IResource& operator=(IResource&&) = default;

    /// Explicitly defaulted copy constructor
    IResource(const IResource&) = default;

    /// Explicitly defaulted copy assignment operator
    IResource& operator=(const IResource&) = default;

	/** Default Destructor */
	virtual ~IResource();

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    std::vector<std::byte> m_data;  ///< Raw bytes of data

protected:
	// --------------------------------------------------------------------- //
	// Protected Member Variables
	// --------------------------------------------------------------------- //
    std::string m_filepath;         ///< Path to the resource

	// --------------------------------------------------------------------- //
	// Protected Member Functions
	// --------------------------------------------------------------------- //
	
	/// Protected default constructor. Constructs an empty resource.
	IResource();

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Get raw data bytes
    /// \return Vector of bytes
    const std::vector<std::byte>& GetData() const { return m_data; }

    /// Get filepath
    /// \return string that contains path to the resource file
    const std::string& GetName() const { return m_filepath; }
};
}