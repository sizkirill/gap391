#pragma once
#include <memory>
#include <string>
/** \file IKeyboard.h */
/** Keyboard input system helper */

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class IKeyboard */
/** Keyboard input system helper. Stores keyboard input for current and previous frames */
class IKeyboard
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    /// \enum KeyCode
    /// Key codes stored by the IKeyboard class
    enum class KeyCode
    {
        kA,             ///< The 'A' key
        kB,             ///< The 'B' key
        kC,             ///< The 'C' key
        kD,             ///< The 'D' key
        kE,             ///< The 'E' key
        kF,             ///< The 'F' key
        kG,             ///< The 'G' key
        kH,             ///< The 'H' key
        kI,             ///< The 'I' key
        kJ,             ///< The 'J' key
        kK,             ///< The 'K' key
        kL,             ///< The 'L' key
        kM,             ///< The 'M' key
        kN,             ///< The 'N' key
        kO,             ///< The 'O' key
        kP,             ///< The 'P' key
        kQ,             ///< The 'Q' key
        kR,             ///< The 'R' key
        kS,             ///< The 'S' key
        kT,             ///< The 'T' key
        kU,             ///< The 'U' key
        kV,             ///< The 'V' key
        kW,             ///< The 'W' key
        kX,             ///< The 'X' key
        kY,             ///< The 'Y' key
        kZ,             ///< The 'Z' key
        
        k1,             ///< The '1' key
        k2,             ///< The '2' key
        k3,             ///< The '3' key
        k4,             ///< The '4' key
        k5,             ///< The '5' key
        k6,             ///< The '6' key
        k7,             ///< The '7' key
        k8,             ///< The '8' key
        k9,             ///< The '9' key
        k0,             ///< The '0' key

        kRETURN,        ///< The 'Return' key
        kESCAPE,        ///< The 'Escape' key
        kBACKSPACE,     ///< The 'Backspace' key
        kTAB,           ///< The 'Tab' key
        kSPACE,         ///< The 'Space' key

        kF1 ,           ///< The 'F1' key
        kF2 ,           ///< The 'F2' key
        kF3 ,           ///< The 'F3' key
        kF4 ,           ///< The 'F4' key
        kF5 ,           ///< The 'F5' key
        kF6 ,           ///< The 'F6' key
        kF7 ,           ///< The 'F7' key
        kF8 ,           ///< The 'F8' key
        kF9 ,           ///< The 'F9' key
        kF10,           ///< The 'F10' key
        kF11,           ///< The 'F11' key
        kF12,           ///< The 'F12' key

        kLCTRL  ,       ///< The 'Left Ctrl' key
        kLSHIFT ,       ///< The 'Left Shift' key
        kLALT   ,       ///< The 'Left Alt' key
        kRCTRL  ,       ///< The 'Right Ctrl' key
        kRSHIFT ,       ///< The 'Right Shift' key
        kRALT   ,       ///< The 'Right Alt' key

        kRIGHT,         ///< The 'Right Arrow' key
        kLEFT ,         ///< The 'Left Arrow' key
        kDOWN ,         ///< The 'Down Arrow' key
        kUP   ,         ///< The 'Up Arrow' key

        kMaxCodes       ///< Unsupported values
    };

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	IKeyboard();

	/** Default Destructor */
	virtual ~IKeyboard() = default;

    /// Initialize the keyboard state
    /// \return true if successful
    virtual bool Init();

    /// Store the key state
    /// \param key - The state keycode to change
    /// \param isPressed - true if the key is down
    virtual void SetKeyState(KeyCode key, bool isPressed);

    /// Check if the key is down this frame
    /// \param key - keycode to check
    /// \return true if the key is down
    virtual bool IsKeyDown(KeyCode key);

    /// Check if the key was just pressed this frame
    /// \param key - keycode to check
    /// \return true if the key was just pressed
    virtual bool IsKeyPressed(KeyCode key);

    /// Check if the key was just released this frame
    /// \param key - keycode to check
    /// \return true if the key was just released
    virtual bool IsKeyReleased(KeyCode key);

    /// Copy the current keyboard state to the previous frame state
    virtual void NextFrame();

    /// Create the keyboard system
    /// \return unique pointer to the keyboard system
    static std::unique_ptr<IKeyboard> Create();

    /// Helper function to parse keycode from string. Can be used in XML for some components
    /// \param data - string to parse
    /// \return the parsed KeyCode
    static KeyCode KeyCodeFromString(const std::string& data);

protected:

    bool m_keyState[static_cast<size_t>(KeyCode::kMaxCodes)];           ///< Array of keys and their states for the current frame
    bool m_previousKeyState[static_cast<size_t>(KeyCode::kMaxCodes)];   ///< Array of keys and their states for the previous frame
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