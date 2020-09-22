#include "IWindow.h"

using yang::IWindow;

IWindow::IWindow()
{
	
}

IWindow::~IWindow()
{
	
}

void yang::IWindow::NextFrame()
{
    if (m_pKeyboard)
        m_pKeyboard->NextFrame();

    if (m_pMouse)
        m_pMouse->NextFrame();
}
