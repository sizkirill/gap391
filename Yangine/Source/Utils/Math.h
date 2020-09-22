#pragma once

/// \file Math.h
/// Math helpers for Yangine

#include <type_traits>
#include <Utils/Typedefs.h>
#include <cmath>

//! namespace detail Contains internal helper math functions
namespace Detail
{
    /// Constexpr Abs function
    /// \tparam T - Type of the argument to call Abs on
    /// \param x - value to get absolute value from
    /// \return absolute value of X
    template <typename T>
    T constexpr Abs(T x)
    {
        static_assert(std::is_arithmetic_v<T>, "Abs is defined only for arithmetic types");
        return x > 0
            ? x
            : -x;
    }

    template <typename T>
    T constexpr Min(T x, T y)
    {
        static_assert(std::is_arithmetic_v<T>, "Min is defined only for arithmetic types");
        return x > y ? y : x;
    }
    
    /// Internal constexpr square root approximation function
    /// \tparam T - Type of the argument and return type
    /// \param x - value to get the square root of
    /// \param curr - current approximation
    /// \param prev - approximation from previous call
    /// \param threshold - the acceptable difference when we stop approximation
    /// \return square root of X approximation
    template <typename T>
    T constexpr SqrtApprox(T x, T curr, T prev, T threshold)
    {
        return Abs(curr - prev) <= threshold
            ? curr
            : SqrtApprox(x, static_cast<T>(0.5) * (curr + x / curr), curr, threshold);
    }
}

//! namespace Math Contains math constants and functions
namespace Math {

    /// PI constant
	static constexpr float kPi = 3.1415926535897932384626433f;

    /// Constexpr version of a square root function
    /// \param threshold - threshold for calculation (default value of 0.01 is enough to provide good accuracy)
    /// \param x - the value to get square root of
    /// \return For a finite and non-negative value of "x", returns an approximation for the square root of "x" - otherwise NaN
    template <typename T>
    T constexpr Sqrt(T x, T threshold = static_cast<T>(0.01))
    {
        static_assert(std::is_floating_point_v<T>, "Value should be a floating point");
        return x >= 0 && x < std::numeric_limits<T>::infinity()
            ? Detail::SqrtApprox(x, x, static_cast<T>(0.), threshold)
            : std::numeric_limits<T>::quiet_NaN();
    }

    /// Converts degrees to radians
    /// \param degrees - angle in degrees to convert
    /// \return angle in radians
	float constexpr ToRadians(float degrees)
	{
		return (kPi * degrees) / 180.f;
	}

    float constexpr AngleDiff(float first, float second)
    {
        return Detail::Min(Detail::Abs(second - first), 2 * Math::kPi - Detail::Abs(second - first));
    }

    /// Converts radians to degrees
    /// \param radians - angle in radians to convert
    /// \return angle in degrees
	float constexpr ToDegrees(float radians)
	{
		return (180.f * radians) / kPi;
	}

    /// Checks if two floating point values are nearly equal
    /// \param first - first value to compare
    /// \param second - second value to compare
    /// \param epsilon - threshold to compare, defaults to 0.001
    /// \return true if values are nearly equal within the given threshold
    template <typename T>
    bool constexpr IsExtremelyClose(T first, T second, T epsilon = static_cast<T>(0.001))
    {
        static_assert(std::is_floating_point_v<T>, "Function is defined for floating point types only");
        return Detail::Abs(first - second) < epsilon;
    }
}

//! namespace yang Contains all Yangine code
namespace yang
{

}