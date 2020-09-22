#pragma once
/** \file Win32Sys.h */
/** Windows OS object specialization for IOpSys */

#include ".\IOpSys.h"
#include <Windows.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class Win32Sys */
/** Windows OS object specialization for IOpSys */
class Win32Sys
	: public IOpSys
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	Win32Sys();

	/** Default Destructor */
	~Win32Sys();

    /// Request window creation
    /// \param title The title of the window
    /// \param width The width of the window (in pixels)
    /// \param height The height of the window (in pixels)
    /// \return Unique pointer to the window
	virtual std::unique_ptr<IWindow> CreateSystemWindow(const char* title, uint32_t width, uint32_t height) override final;

	/// Return the name of operating system
    /// \return "Windows"
	virtual const char* GetSystemName() const override final { return "Windows"; }

    /// Return the information about system RAM
    // \return RAM information. \see yang::IOpSys::MemoryInfo
	virtual MemoryInfo GetMemoryInfo() const override final;

    /// Return the information about available Hard disks
    /// \return Vector of HDD information. \see yang::IOpSys::HDDInfo
	virtual std::vector<HDDInfo> GetHDDInfo() const override final;

    /// List the contents of the directory in the console. For debug purposes
    /// \param directory - path to a directory
    /// \param recursive - if true also lists the contents of subdirectories
    virtual void ListDirectory(const char* directory, bool recursive) const override final;

    /// Set the console text color
    /// \param color - Color to set
    /// \param intensity - Color intensity to set 
	virtual void SetConsoleColor(ConsoleColor color, ColorIntensity intensity) const override final;


private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	HANDLE m_consoleHandle;     ///< Handle to the Windows console

    /// Array that handles conversion from the ConsoleColor to the Windows specific console text color
	static constexpr WORD kConsoleColors[static_cast<size_t>(ConsoleColor::MAX_COLORS)] =
	{
		0,
		FOREGROUND_RED,
		FOREGROUND_GREEN,
		FOREGROUND_BLUE,
		FOREGROUND_RED | FOREGROUND_GREEN,
		FOREGROUND_GREEN | FOREGROUND_BLUE,
		FOREGROUND_RED | FOREGROUND_BLUE,
		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN
	};

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}