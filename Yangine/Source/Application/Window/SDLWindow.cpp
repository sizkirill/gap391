#include "SDLWindow.h"
#include <Utils/Logger.h>

using yang::SDLWindow;
using yang::IKeyboard;
using yang::IMouse;

SDLWindow::SDLWindow()
	: m_initialized(false)
	, m_pSDLWindow(nullptr, &SDL_DestroyWindow)
{
	
}

SDLWindow::~SDLWindow()
{
	m_pSDLWindow.reset();
	if (m_initialized)
	{
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
	SDL_Quit();
}

bool yang::SDLWindow::Init(const char* title, uint32_t width, uint32_t height)
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		LOG(Error, "SDL_InitSubSystem(SDL_INIT_VIDEO) failed, %s", SDL_GetError());
		return false;
	}

	m_pSDLWindow.reset(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN));

	if (!m_pSDLWindow)
	{
		LOG(Error, "SDL_Window failed to initialize, %s", SDL_GetError());
		return false;
	}

	m_initialized = true;
	return true;
}

bool yang::SDLWindow::ProcessEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT ||
			(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
		{
			return false;
		}
        
        else if (m_pKeyboard &&
            (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP))
        {
            IKeyboard::KeyCode keyCode = ConvertKeyCode(event.key.keysym.scancode);
            m_pKeyboard->SetKeyState(keyCode, event.type == SDL_KEYDOWN);
        }

        else if (m_pMouse &&
            (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP))
        {
            IMouse::MouseButton button = ConvertMouseButton(event.button.button);
            m_pMouse->SetButtonState(button, event.type == SDL_MOUSEBUTTONDOWN);
        }

        else if (m_pMouse &&
            (event.type == SDL_MOUSEMOTION))
        {
            m_pMouse->SetMousePosition(event.button.x, event.button.y);
        }

        else if (m_pMouse &&
            (event.type == SDL_MOUSEWHEEL))
        {
            m_pMouse->ProcessWheelEvent(event.wheel.direction == SDL_MOUSEWHEEL_NORMAL, event.wheel.x, event.wheel.y);
        }
	}
	return true;
}

void* yang::SDLWindow::GetNativeWindow() const
{
	return m_pSDLWindow.get();
}

yang::IVec2 yang::SDLWindow::GetDimensions() const
{
    int x = 0;
    int y = 0;
    SDL_GetWindowSize(m_pSDLWindow.get(), &x, &y);
    return IVec2(x,y);
}

IKeyboard::KeyCode yang::SDLWindow::ConvertKeyCode(uint32_t code)
{
    if (code >= SDL_SCANCODE_A && code <= SDL_SCANCODE_Z)
    {
        return static_cast<IKeyboard::KeyCode>(code - SDL_SCANCODE_A + static_cast<int>(IKeyboard::KeyCode::kA));
    }
    if (code >= SDL_SCANCODE_1 && code <= SDL_SCANCODE_0)
    {
        return static_cast<IKeyboard::KeyCode>(code -
            SDL_SCANCODE_1 + static_cast<int>(IKeyboard::KeyCode::k1));
    }
    if (code >= SDL_SCANCODE_RETURN && code <= SDL_SCANCODE_SPACE)
    {
        return static_cast<IKeyboard::KeyCode>(code -
            SDL_SCANCODE_RETURN + static_cast<int>(IKeyboard::KeyCode::kRETURN));
    }
    if (code >= SDL_SCANCODE_F1 && code <= SDL_SCANCODE_F12)
    {
        return static_cast<IKeyboard::KeyCode>(code -
            SDL_SCANCODE_F1 + static_cast<int>(IKeyboard::KeyCode::kF1));
    }
    if (code >= SDL_SCANCODE_LCTRL && code <= SDL_SCANCODE_RALT)
    {
        return static_cast<IKeyboard::KeyCode>(code -
            SDL_SCANCODE_LCTRL + static_cast<int>(IKeyboard::KeyCode::kLCTRL));
    }
    if (code >= SDL_SCANCODE_RIGHT && code <= SDL_SCANCODE_UP)
    {
        return static_cast<IKeyboard::KeyCode>(code -
            SDL_SCANCODE_RIGHT + static_cast<int>(IKeyboard::KeyCode::kRIGHT));
    }

    return IKeyboard::KeyCode::kMaxCodes;

}

IMouse::MouseButton yang::SDLWindow::ConvertMouseButton(uint32_t code)
{
    if (code >= SDL_BUTTON_LEFT && code <= SDL_BUTTON_X2)
    {
        return static_cast<IMouse::MouseButton>(code - 1);
    }
    return IMouse::MouseButton::kMaxButtons;
}
