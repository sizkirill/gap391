#include "Win32Sys.h"

#include <Application/Window/SDLWindow.h>
#include <Utils/Logger.h>
#include <vector>

using yang::Win32Sys;

Win32Sys::Win32Sys()
	:m_consoleHandle(GetStdHandle(STD_OUTPUT_HANDLE))
{
	
}

Win32Sys::~Win32Sys()
{
	
}

void yang::Win32Sys::SetConsoleColor(ConsoleColor color, ColorIntensity intensity) const
{
	WORD consoleColor = kConsoleColors[static_cast<size_t>(color)];

	if (intensity == ColorIntensity::kLight)
		consoleColor |= FOREGROUND_INTENSITY;

	SetConsoleTextAttribute(m_consoleHandle, consoleColor);
}

yang::Win32Sys::MemoryInfo yang::Win32Sys::GetMemoryInfo() const
{
	MEMORYSTATUSEX buffer;
    buffer.dwLength = sizeof(buffer);

	if (!GlobalMemoryStatusEx(&buffer))
	{
		LOG(Error, "Unable to retrieve memory information. Error code: %d", GetLastError());
        return { 0,0,0 };
	}

    return { static_cast<uint8_t>(buffer.dwMemoryLoad), buffer.ullTotalPhys / (1024 * 1024), buffer.ullAvailPhys / (1024*1024) };
}

std::vector<yang::Win32Sys::HDDInfo> yang::Win32Sys::GetHDDInfo() const
{
    DWORD flags = GetLogicalDrives();
    if (!flags)
    {
        LOG(Error, "Unable to retrieve HDD information. Error code: %d", GetLastError());
        return {};
    }

    std::vector<char> drives;
    for (size_t i = 0; i < sizeof(flags) * 8; ++i, flags >>= 1)
    {
        if (flags & 0x1)
        {
            drives.push_back('A' + static_cast<char>(i));
        }
    }

    std::string root = "C:\\";
    std::vector<HDDInfo> output;

    for (auto drive : drives)
    {
        root[0] = drive;
        LPCSTR rootPath = root.c_str();
        ULARGE_INTEGER freeBytesAvailableToCaller;
        ULARGE_INTEGER totalNumberOfBytes;
        ULARGE_INTEGER totalNumberOfFreeBytes;

        if (!GetDiskFreeSpaceExA(rootPath, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            LOG(Error, "Unable to retrieve HDD space. Error code: %d", GetLastError());
            return {};
        }

        output.push_back({ drive, totalNumberOfBytes.QuadPart / (1024 * 1024), totalNumberOfFreeBytes.QuadPart / (1024 * 1024) });
    }

    return output;
}

void yang::Win32Sys::ListDirectory(const char* directory, bool recursive) const
{
    std::string command = "dir /b ";
    if (recursive)
        command.append("/s ");
    if (directory)
        command.append(directory);
    
    system(command.c_str());
}

std::unique_ptr<yang::IWindow> yang::Win32Sys::CreateSystemWindow(const char* title, uint32_t width, uint32_t height)
{
	std::unique_ptr<IWindow> pWindow = std::make_unique<SDLWindow>();
	if (pWindow->Init(title, width, height) == false)
	{
		LOG(Error, "yang::Win32Sys::CreateSystemWindow failed! Error: %s", SDL_GetError());
		return nullptr;
	}
	return pWindow;
}
