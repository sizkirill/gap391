#pragma once
#include "Typedefs.h"

/// \file Color.h
/// Color description helpers for Yangine

//! namespace yang Contains all Yangine code
namespace yang
{

class IColor;

/// \class FColor
/// Describes a color with floating point value in range [0.0, 1.0]
class FColor
{
public:
    f32 r = 0.f;    ///< red component value in range [0.0, 1.0]
    f32 g = 0.f;    ///< green component value in range [0.0, 1.0]
    f32 b = 0.f;    ///< blue component value in range [0.0, 1.0]
    f32 a = 1.0f;   ///< transparency component value in range [0.0, 1.0]

    /// Default constructor
    FColor() = default;

    /// Constructor
    /// \param r - red value in range [0.0, 1.0]
    /// \param g - green value in range [0.0, 1.0]
    /// \param b - blue value in range [0.0, 1.0]
    /// \param a - transparency value in range [0.0, 1.0]
    FColor(f32 r, f32 g, f32 b, f32 a);

    /// Convert to integer color
    /// \return converted color
    IColor ToIColor() const;

    /// Get red component
    f32 Red() const { return r; }
    /// Get green component
    f32 Green() const { return g; }
    /// Get blue component
    f32 Blue() const { return b; }
    /// Get transparency component
    f32 Alpha() const { return a; }
};

/// \class IColor
/// Describes a color with integer component values in range [0-255]
class IColor
{
public:
    uint32_t m_color = 0;   ///< 4 integer components of a color in a one unsigned int

    /// Default constructor
    constexpr IColor() = default;

    /// Constructor
    /// \param r - red value in range [0 - 255]
    /// \param g - green value in range [0 - 255]
    /// \param b - blue value in range [0 - 255]
    /// \param a - transparency value in range [0 - 255]
    constexpr IColor(ui8 r, ui8 g, ui8 b, ui8 a) : m_color(r << 24 | g << 16 | b << 8 | a) {};

    /// Constructor
    /// \param color - unsigned int that contains all 4 components
    constexpr IColor(uint32_t color) : m_color(color) {};

    /// Convert to floating point color
    /// \return converted color
    FColor ToFColor() const;

    /// Get red component value
    constexpr uint8_t Red() const { return (m_color >> 24) & 0xFF; }
    /// Get green component value
    constexpr uint8_t Green() const { return (m_color >> 16) & 0xFF; }
    /// Get blue component value
    constexpr uint8_t Blue() const { return (m_color >> 8) & 0xFF; }
    /// Get transparency component value
    constexpr uint8_t Alpha() const { return m_color & 0xFF; }

    static constexpr uint32_t kWhite = 0xffffffff;
    static constexpr uint32_t kBlack = 0x000000ff;

    static constexpr IColor White() { return yang::IColor(kWhite); }
    static constexpr IColor Black() { return yang::IColor(kBlack); }
};

} // namespace yang