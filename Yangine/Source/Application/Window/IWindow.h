#pragma once
#include <cstdint>
#include <Utils/Vector2.h>
#include <Application/Input/IKeyboard.h>
#include <Application/Input/IMouse.h>
/** \file IWindow.h */
/** Interface for a window */


//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class IWindow */
/** Interface to handle system window */
class IWindow
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	IWindow();

	/** Default Destructor */
	virtual ~IWindow();

    /// Initialize a window with given parameters
    /// \param title - Window title
    /// \param width - Window width in pixels
    /// \param height - Window height in pixels
    /// \return true if successful
    virtual bool Init(const char* title, uint32_t width, uint32_t height) = 0;

    /// Handle operating system events
    /// \return true if the game should continue to run
    virtual bool ProcessEvents() = 0;

    /// Get the underlying window object
    /// \return pointer to the underlying window object
    virtual void* GetNativeWindow() const = 0;

    /// Processes keyboard and mouse to the next frame. \see IMouse::NextFrame \see IKeyboard::NextFrame
    virtual void NextFrame();

    /// \brief Get dimensions of the window
    /// \return IVec2 that contains window dimensions
    virtual IVec2 GetDimensions() const = 0;

protected:
    std::unique_ptr<IKeyboard> m_pKeyboard;     ///< Keyboard owned by the window
    std::unique_ptr<IMouse> m_pMouse;           ///< Mouse owned by the window

    /// Convert middleware-specific keycode to yang keycode
    /// \param code - key code returned by middleware
    /// \return converted KeyCode \see yang::IKeyboard::KeyCode
    virtual IKeyboard::KeyCode ConvertKeyCode(uint32_t code) = 0;

    /// Convert middleware-specific mouse button to yang mouse button
    /// \param code - mouse button code returned by middleware
    /// \return converted MouseButton \see yang::IMouse::MouseButton
    virtual IMouse::MouseButton ConvertMouseButton(uint32_t code) = 0;
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

    /// Attach keyboard to the window
    /// \param pKeyboard - unique pointer to a keyboard object
    virtual void AttachKeyboard(std::unique_ptr<IKeyboard> pKeyboard) { m_pKeyboard.reset(pKeyboard.release()); };

    /// Attach mouse to the window
    /// \param pMouse - unique pointer to a mouse object
    virtual void AttachMouse(std::unique_ptr<IMouse> pMouse) { m_pMouse.reset(pMouse.release()); }

    /// Get the keyboard associated with the window
    /// \return pointer to an associated keyboard object
    virtual IKeyboard* GetKeyboard() { return m_pKeyboard.get(); }

    /// Get the mouse associated with the window
    /// \return pointer to an associated mouse object
    virtual IMouse* GetMouse() { return m_pMouse.get(); }

};
}