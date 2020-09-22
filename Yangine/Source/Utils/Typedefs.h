#pragma once
/// \file Typedefs.h
/// Typedefs for use in Yangine
#include <stdint.h>
#include <limits>

#undef max

//! namespace yang Contains all Yangine code
namespace yang
{
    /// Alias for unsigned 32 bit integer
    using ui32 = uint32_t;
    /// Alias for 64 bit unsigned integer
    using ui64 = uint64_t;
    /// Alias for 16 bit unsigned integer
    using ui16 = uint16_t;
    /// Alias for 8 bit unsigned integer
    using ui8 = uint8_t;
    /// Alias for 8 bit integer
    using i8 = int8_t;
    /// Alias for 16 bit integer
    using i16 = int16_t;
    /// Alias for 32 bit integer
    using i32 = int32_t;
    /// Alias for 64 bit integer
    using i64 = int64_t;
    /// Alias for a float type
    using f32 = float;
    /// Alias for a double type
    using f64 = double;
    /// Alias for any kind of Id
    using Id = uint32_t;

    /// Invalid float constant
    constexpr float kInvalidFloat = std::numeric_limits<float>::max();
    
    /// Invalid any numeric type constant
    template <class Type>
    constexpr Type kInvalidValue = std::numeric_limits<Type>::max();

    /// \return true if a value is valid
    template <class Type>
    constexpr bool IsValid(Type value) { return value != kInvalidValue<Type>; }
}