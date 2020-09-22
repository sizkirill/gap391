#include "Logger.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <Lua/lua.hpp>

using yang::Logger;

Logger* yang::Logger::Get()
{
	static Logger s_instance;
	return &s_instance;
}

void yang::Logger::Print(std::future<void> future)
{
	using namespace std::chrono_literals;

	while (future.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
	{
		std::unique_lock<std::mutex> lock(m_logMutex);
		m_hasMoreWork.wait(lock, [this, &future]() {return !m_buffer.empty() || future.valid(); });
		std::vector<Log> buffer = std::move(m_buffer);
		lock.unlock();

		for (auto& log : buffer)
		{
			const std::string& categoryName = log.m_categoryName;
			const LogCategory& logCategory = m_logCategories[categoryName];

			m_pOpSys->SetConsoleColor(logCategory.m_color, logCategory.m_colorIntensity);
			std::cout << log.m_logString;
			m_outFile << log.m_logString;
			m_pOpSys->SetConsoleColor(ConsoleColor::kWhite, ColorIntensity::kDark);
		}
		buffer.clear();

	}
}


void yang::Logger::AddCategory(const std::string& name, size_t severity, ConsoleColor color, ColorIntensity colorIntensity)
{
	m_logCategories.insert({ name, { severity, color, colorIntensity } });
}

bool yang::Logger::Init(IOpSys* pOpSys)
{
	using std::chrono::system_clock;

	m_pOpSys = pOpSys;
	std::string outputFile = "./Temp/Yang_Log_" + std::to_string(system_clock::to_time_t(system_clock::now())) + ".log";
	m_outFile.open(outputFile.c_str(), 
		std::ios_base::out | std::ios_base::trunc);

	std::future futureObj = m_exitSignal.get_future();
	m_loggerThread = std::thread(&Logger::Print, this, std::move(futureObj));

	if (!m_outFile.is_open())
	{
		// Log ??
		return false;
	}
	return true;
}

void yang::Logger::Finish()
{
	// Waiting for logger thread to finish properly
	while (!m_buffer.empty())
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(0.01s);
	}

	m_exitSignal.set_value();
	m_loggerThread.join();
    m_buffer.clear();
    if (m_outFile.is_open())
    {
        m_outFile.close();
    }
}

std::string yang::Logger::Timestamp() const
{
	using namespace std::chrono;

	auto now = high_resolution_clock::now();

	auto microsec = duration_cast<microseconds>(now.time_since_epoch()).count();
	auto millisec = (microsec / 1000) % 1000;
	microsec %= 1000;

	// get printable result:
	auto nowTime = system_clock::to_time_t(system_clock::now());
	std::stringstream ss;
	tm time;
	localtime_s(&time, &nowTime);
	ss << std::put_time(&time, "%d-%m-%Y %X:") << millisec << ":" << microsec;
	return ss.str();
}

Logger::Logger()
	:m_pOpSys(nullptr)
{
	memset(m_messageBuffer, 0, sizeof(m_messageBuffer));
}

Logger::~Logger()
{
}

yang::Log::Log(const std::string& catName, std::string&& logString)
	:m_categoryName(catName)
	,m_logString(std::move(logString))
{
}

int Lua::Logger_Log(lua_State* pState)
{
	// Get the message from the stack
	const char* pMessage = lua_tostring(pState, -1);

	// Grab debug info from the stack
	lua_Debug ar;
	lua_getstack(pState, 1, &ar);
	lua_getinfo(pState, "Slnt", &ar);

	// Shorten file name
	static std::string file;
	file = ar.short_src;
	file = file.substr(file.rfind('/') + 1);

	// Log it!
	Logger::Get()->AddLog("Lua", ar.currentline, file.c_str(), pMessage);

	return 0;
}
