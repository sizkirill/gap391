#include "IOpSys.h"
#include <Utils/Logger.h>

// Option 1: Use 'full path'
// Option 2: Use 'relative path'
#include "Win32Sys.h"

using yang::IOpSys;

IOpSys::IOpSys()
{
	
}

IOpSys::~IOpSys()
{
	
}

std::unique_ptr<IOpSys> yang::IOpSys::Create()
{
#ifdef _WIN32
	return std::make_unique<Win32Sys>();
#else
	//TODO: ERROR
	return nullptr;
#endif
}

void yang::IOpSys::MemoryInfo::Log()
{
    LOG(RAMInfo, "_______RAM INFO SECTION_______");
    LOG(RAMInfo, "Memory load percentage: %d", m_memoryLoad);
    LOG(RAMInfo, "Total system memory: %d GB, %d MB", m_totalMemory / 1024, m_totalMemory % 1024);
    LOG(RAMInfo, "Available system memory: %d GB, %d MB", m_availableMemory / 1024, m_availableMemory % 1024);
}

void yang::IOpSys::HDDInfo::Log()
{
    LOG(HDDInfo, "_______DISK DRIVE %c:/ INFO_______", m_drive);
    LOG(HDDInfo, "Total HDD space: %d GB, %d MB", m_totalSpace / 1024, m_totalSpace % 1024);
    LOG(HDDInfo, "Available HDD space: %d GB, %d MB", m_availableSpace / 1024, m_availableSpace % 1024);
}
