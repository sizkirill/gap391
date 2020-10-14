#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <deque>
#include <future>
#include <thread>
#include <atomic>
#include <mutex>
#include <string_view>
#include <condition_variable>

#include <Application/OS/IOpSys.h>

/** \file Logger.h */
/** Logger class description */

/// \def LOG_CATEGORY(Name, Severity, Color, Intensity)
/// Defines Log category
/// \a Name - name of the log category
/// \a Severity - category severity
/// \a Color - color to log
/// \a Intensity - intensity of the color to log
#define LOG_CATEGORY(Name, Severity, Color, Intensity) \
	yang::Logger::Get()->AddCategory(#Name, Severity, yang::ConsoleColor::k##Color, yang::ColorIntensity::k##Intensity); \
	
/// \def LOG(Category, format, ...)
/// Adds log to a logging queue
/// \a Category - log category to log in
/// \a format - string to log
/// \a ... - format arguments for the string above
#define LOG(Category, format, ...) \
	yang::Logger::Get()->AddLog(#Category, __LINE__, __FILE__, format, __VA_ARGS__);

// Little gross way of logging once xD
/// \def LOG_ONCE(Category, Identifier, format, ...)
/// Logs the log one time
/// \a Category - category to log
/// \a Identifier - unique identifier of the log
/// \a format - string to log
/// \a ... - format arguments for the string above
#define LOG_ONCE(Category, Identifier, format, ...) \
    static bool hasLogged##Category##Identifier = []() {LOG(Category, "Description: %s. \n %s", #Identifier, format); return true;}();


//! \namespace yang Contains all Yangine code
namespace yang
{

/// \struct Log
/// Contains data for a single Log message
struct Log
{
	std::string m_categoryName;     ///< Name of the log category
	std::string m_logString;        ///< Formatted string to log

    /// Constructor
    /// \param catName - log category name
    /// \param logString - formatted string to log
	Log(const std::string& catName, std::string&& logString);
};

/// \struct LogCategory
/// Contains information about log category
struct LogCategory
{
	size_t m_severity;                  ///< Log category severity
	ConsoleColor m_color;               ///< Log color
	ColorIntensity m_colorIntensity;    ///< Log color intensity
};

class IOpSys;
/** \class Logger */
/** Writes logs to file and console. This class is Singleton */
class Logger
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Get singleton instance of the class
	static Logger* Get();

    /// Function that prints logs to the console and to the file. Called from different thread
	void Print();

    /// Adds the log category to the lookup table
    /// \param name - Log category name
    /// \param severity - Log category severity
    /// \param color - Log category color
    /// \param colorIntensity - Log cateogry color intensity
	void AddCategory(const std::string& name, size_t severity, ConsoleColor color, ColorIntensity colorIntensity);

    /// Adds log to the logging queue
    /// \tparam FormatArgs - Argument types to format the log string
    /// \param categoryName - Name of the category to log the string to
    /// \param line - Line where the LOG macro was called
    /// \param file - Path to the file where LOG macro was called
    /// \param format - The actual string to log
    /// \param args - Arguments to format the above string with
	template <class ...FormatArgs>
	void AddLog(const std::string& categoryName, int line, const char* file, const char* format, FormatArgs ...args);

    /// Initializes the logger
    /// \param pOpSys - Operation System object
    /// \return true if initialized successfully
	bool Init(IOpSys* pOpSys);

    /// Sets up the exit signal value, waits for the thread to finish it's work and cleans up memory
	void Finish();

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	IOpSys* m_pOpSys;                                                 ///< Operating system object to handle the colored console output
	std::atomic<bool> m_exitSignal;                                   ///< Exit signal for the logger thread
	std::thread m_loggerThread;                                       ///< Thread where logging actually happens

	std::ofstream m_outFile;                                          ///< Path to the file where to output logs

	std::unordered_map<std::string, LogCategory> m_logCategories;     ///< Lookup map for log categories

    std::mutex m_logMutex;                                            ///< Mutex for the log buffer
	std::mutex m_threadMutex;										  ///< Mutex for logging from different threads
	std::vector<Log> m_buffer;                                        ///< Vector of actual logs. Main thread writes in it, logger thread reads from it
	std::condition_variable m_hasMoreWork;                            ///< Condition variable that signals logger thread when to sleep and when to wake up

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    /// Get the current timestamp
    /// \return formatted timestamp
    std::string Timestamp() const;

	/** Default Constructor */
	Logger();

	/** Default Destructor */
	~Logger();

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //
};

template <class ...FormatArgs>
inline void Logger::AddLog(const std::string& categoryName, int line, const char* file, const char* format, FormatArgs ...args)
{
	if (m_logCategories.count(categoryName))
	{
		char buf[256];
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, format, args...);

		std::stringstream ss;

		std::string_view filename = file;
		std::string_view shortFileName = filename.substr(filename.rfind('\\') + 1);

		ss << "[" << Timestamp() << "]" << " [" << categoryName << "]: (" << shortFileName << ":" << line << ") " << buf << std::endl;

		m_logMutex.lock();
		m_buffer.emplace_back(categoryName , ss.str());
        m_logMutex.unlock();

		m_hasMoreWork.notify_one();
	}
}

} // namespace yang


// Lua stuff

struct lua_State;

//! namespace Lua Contains lua environment specific functions
namespace Lua
{
    /// Function to use the Logger in the lua environment
	int Logger_Log(lua_State* pState);
}