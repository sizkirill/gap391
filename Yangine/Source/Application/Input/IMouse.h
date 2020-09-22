#pragma once
#include <memory>
#include <Utils/Math.h>
#include <Utils/Vector2.h>
/** \file IMouse.h */
/** Mouse input system helper */


//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class IMouse */
/** Mouse input system helper. Stores mouse input for current and previous frames */
class IMouse
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    /// \enum MouseButton
    /// Buttons stored by the IMouse class
    enum class MouseButton
    {
        kLeft  ,        ///< Left mouse button
        kMiddle,        ///< Wheel button
        kRight ,        ///< Right mouse button
        kOne   ,        ///< First extra mouse button
        kTwo   ,        ///< Second extra mouse button
        kMaxButtons,    ///< Maximum number of buttons handled by this system
    };

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	IMouse();

	/** Default Destructor */
	virtual ~IMouse() = default;

    /// Initializes the starting mouse state
    /// \return true if initialized successfully
    virtual bool Init();

    /// Store the button state
    /// \param button - The state MouseButton to change
    /// \param isPressed - true if the key is down
    virtual void SetButtonState(MouseButton button, bool isPressed);

    /// Check if the button is down this frame
    /// \param button - button to check
    /// \return true if the key is down
    virtual bool IsButtonDown(MouseButton button);

    /// Check if the button was just pressed this frame
    /// \param button - button to check
    /// \return true if the key was just pressed
    virtual bool IsButtonPressed(MouseButton button);

    /// Check if the button was just released this frame
    /// \param button - button to check
    /// \return true if the key was just released
    virtual bool IsButtonReleased(MouseButton button);

    /// Copy the current mosue state to the previous frame state
    virtual void NextFrame();

    /// Processes scrolling of the mouse wheel
    /// \param isNormal - True if the wheel direction is not inverted. (On some platforms the generated mouse wheel can have inverted scroll amount values)
    /// \param horizontalAmount - Amount of horizontal scroll (whatever this is..)
    /// \param verticalAmount - Amoutn of vertical scroll
    virtual void ProcessWheelEvent(bool isNormal, i32 horizontalAmount, i32 verticalAmount);

    /// Create the mouse system
    /// \return unique pointer to the mouse system
    static std::unique_ptr<IMouse> Create();

protected:
    bool m_buttonState[static_cast<size_t>(MouseButton::kMaxButtons)];          ///< Array of button states for the current frame
    bool m_previousButtonState[static_cast<size_t>(MouseButton::kMaxButtons)];  ///< Array of button states for the previous frame
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    IVec2 m_position;                                                           ///< Current mouse position on the screen (in pixels)

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Get the current mouse position
    /// \return mouse position in pixels relative to the top left corner of the screen
    IVec2 GetMousePosition() const { return m_position; }

    /// Sets the current mouse position. Used by Window to update the mouse position information.
    /// \param x - x position on the screen relative to the left side of the screen
    /// \param y - y position on the screen relative to the top of the screen
    void SetMousePosition(int x, int y);
};
}