#pragma once

#include <memory>
#include <vector>
/** \file IOpSys.h */
/** Interface for the operating system object that is used */


//! \namespace yang Contains all Yangine code
namespace yang
{

/// \enum ConsoleColor
/// Possible colors for the text in the console
enum class ConsoleColor
{
	kBlack,        ///< Black color
	kRed,          ///< Red color
	kGreen,        ///< Green color
	kBlue,         ///< Blue color
	kYellow,       ///< Yellow color
	kCyan,         ///< Cyan color
	kMagenta,      ///< Magenta color
	kWhite,        ///< White color
	MAX_COLORS     ///< Maximum supported colors
};

/// \enum ColorIntensity
/// Color intensity specifiers for the ConsoleColor
enum class ColorIntensity
{
	kDark,          ///< Make the color dark
	kLight          ///< Make the color light
};

class IWindow;
/** \class IOpSys */
/** Interface for the operating system */
class IOpSys
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    /// \struct MemoryInfo
    /// Describes information about physical memory
    struct MemoryInfo
    {
        uint8_t m_memoryLoad;         ///< Current memory load percentage
        uint64_t m_totalMemory;       ///< Total memory in MB
        uint64_t m_availableMemory;   ///< Current available memory in MB
        void Log();                   ///< Logs the memory info using Logger. For debug purposes
    };

    /// \struct HDDInfo
    /// Describes information about HDD
    struct HDDInfo
    {
        char m_drive;                 ///< Disk drive letter
        uint64_t m_totalSpace;        ///< Total disk space in MB
        uint64_t m_availableSpace;    ///< Free disk space in MB
        void Log();                   ///< Logs the HDD info using Logger. For debug purposes
    };

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //
	/** Default Constructor */
	IOpSys();

	/** Default Destructor */
	virtual ~IOpSys();

	// --------------------------------------------------------------------- //
	// System Information
	// --------------------------------------------------------------------- //

    /// Return the operating system name.
    /// \return name of the operating system
	virtual const char* GetSystemName() const = 0;

    /// Return the information about system RAM
    /// \return RAM information. \see yang::IOpSys::MemoryInfo
	virtual MemoryInfo GetMemoryInfo() const = 0;

    /// Return the information about available Hard disks
    /// \return Vector of HDD information. \see yang::IOpSys::HDDInfo
    virtual std::vector<HDDInfo> GetHDDInfo() const = 0;

    /// List the contents of the directory in the console. For debug purposes
    /// \param directory - path to a directory
    /// \param recursive - if true also lists the contents of subdirectories
    virtual void ListDirectory(const char* directory, bool recursive) const = 0;

    /// Set the console text color
    /// \param color - Color to set
    /// \param intensity - Color intensity to set 
	virtual void SetConsoleColor(ConsoleColor color, ColorIntensity intensity) const = 0;

    /// Create the operating system to be used.
    /// \return Unique pointer to the operating system object
	static std::unique_ptr<IOpSys> Create();

    /// Request window creation
    /// \param title The title of the window
    /// \param width The width of the window (in pixels)
    /// \param height The height of the window (in pixels)
    /// \return Unique pointer to the window
	virtual std::unique_ptr<IWindow> CreateSystemWindow(const char* title, uint32_t width, uint32_t height) = 0;
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