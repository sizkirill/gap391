#pragma once
/** \file SDLWindow.h */
/** SDL_Window specialization for IWindow interface */

#include ".\IWindow.h"

#include <memory>

#include <SDL/SDL.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class SDLWindow */
/** SDL_Window specialization for IWindow interface */
class SDLWindow
	: public IWindow
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	SDLWindow();

	/** Default Destructor */
	~SDLWindow();

    /// Initialize a window with given parameters
    /// \param title - Window title
    /// \param width - Window width in pixels
    /// \param height - Window height in pixels
    /// \return true if successful
	virtual bool Init(const char* title, uint32_t width, uint32_t height) override final;

    /// Handle operating system events
    /// \return true if the game should continue to run
	virtual bool ProcessEvents() override final;

    /// Get the underlying window object representation
    /// \return pointer to the SDL_Window
	virtual void* GetNativeWindow() const override final;

	/// \brief Get dimensions of the window
	/// \return IVec2 that contains window dimensions
	virtual IVec2 GetDimensions() const override final;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
	bool m_initialized;                                                         ///< True if window was initialized
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_pSDLWindow;     ///< Unique pointer to the underlying SDL_Window
    
	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

    /// Convert SDL-specific keycode to yang keycode
    /// \param code - key code returned by SDL
    /// \return converted KeyCode \see yang::IKeyboard::KeyCode
    virtual IKeyboard::KeyCode ConvertKeyCode(uint32_t code) override final;

    /// Convert SDL-specific mouse button to yang mouse button
    /// \param code - mouse button code returned by SDL
    /// \return converted MouseButton \see yang::IMouse::MouseButton
    virtual IMouse::MouseButton ConvertMouseButton(uint32_t code) override final;

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

};
}