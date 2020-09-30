#pragma once

/// \file Vector2.h
/// Vector2 class description

#include <Utils/Math.h>
#include <algorithm>

namespace tinyxml2
{
    class XMLElement;
}

//! namespace yang Contains all Yangine code
namespace yang
{

template <typename T>
class Rectangle;

/// \class Vector2
/// \tparam T - type of the value for a vector. Must be arithmetic
/// Describes point on a 2D plane
template <typename T>
class Vector2
{
    static_assert(std::is_arithmetic_v<T>, "Vector2 template parameter should be arithmetic");

    /// Alias for enabling things only for floating point values
    template<typename U = T>
    using IsFloatingPoint = std::enable_if_t<std::is_floating_point_v<U>>;

    template<typename U = T>
    using IsIntegral = std::enable_if_t<std::is_integral_v<U>>;
public:

    /// Typedef for TypeTraits
    typedef T ValueType;

    // yep, for simplicity - giving up on standards
    T x;    ///< X value
    T y;    ///< Y value

    /// Default constructor
    constexpr Vector2() = default;

    /// Constexpr constructor
    /// \param _x - X value
    /// \param _y - Y value
    constexpr Vector2(T _x, T _y);

    /// Explicitly defaulted copy constructor
    constexpr Vector2(const Vector2&) = default;

    /// Explicitly defaulted copy assignment operator
    constexpr Vector2& operator=(const Vector2&) = default;

    /// Default destructor
    ~Vector2() = default;

    /// Constructor that converts Vector2 of other type to Vector2 of this type
    /// \tparam U - template parameter of other Vector2
    /// \param other - Vector2 to convert
    template<typename U>
    constexpr Vector2<T>(const Vector2<U>& other);

    /// Determines whether a point is inside a rectangle
    /// \param rect - the rectangle to check
    /// \return true if the point is inside rect
    bool IsInside(const Rectangle<T>& rect) const;

    /// Constexpr version of length, uses constexpr sqrt implementation.
    /// \return length of the line from {0,0} to this point
    template <typename = IsFloatingPoint<T>>
    [[deprecated]] constexpr T ConstexprLength() const
    {
        return Math::Sqrt(x * x + y * y);
    }

    /// Get length
    /// \return length of the line from {0,0} to this point
    template <typename = IsFloatingPoint<T>>
    constexpr T Length() const
    {
        return Math::Sqrt(SqrdLength());
    }

    /// <summary>
    /// Makes a normalized vector from angle
    /// </summary>
    /// <param name="angle">angle in radians clockwise from (1,0)</param>
    /// <returns></returns>
    template <typename = IsFloatingPoint<T>>
    static Vector2<T> FromAngle(T angle)
    {
        return {std::cos(angle), std::sin(angle)};
    }

    /// \brief Multiplies vectors component-wise.
    /// Every component in the result is a component of this vector multiplied by the same component of other.
    /// \param other - vector to scale by
    constexpr void Scale(Vector2 other)
    {
        x *= other.x;
        y *= other.y;
    }

    /// \brief Multiplies two vectors component - wise.
    /// Every component in the result is a component of lhs vector multiplied by the same component of rhs.
    /// \param lhs - vector to scale
    /// \param rhs - amount to scale
    /// \return scaled vector
    static constexpr Vector2 Scale(Vector2 lhs, Vector2 rhs)
    {
        lhs.Scale(rhs);
        return lhs;
    }

    template <typename = IsFloatingPoint<T>>
    static constexpr Vector2 Normalized(Vector2 v)
    {
        v.Normalize();
        return v;
    }

    /// Constexpr version of normalize the vector (make the length = 1)
    /// \return the normalized vector
    template <typename = IsFloatingPoint<T>>
    [[deprecated]] constexpr Vector2 Normalize() const
    {
        T length = ConstexprLength();
        return Vector2(x / length, y / length);
    }

    /// Normalize the vector (make the length = 1)
    template <typename = IsFloatingPoint<T>>
    constexpr void Normalize()
    {
        if (x != static_cast<T>(0) || y != static_cast<T>(0))
        {
            T length = Length();
            x /= length;
            y /= length;
        }
    }

    /// Linearly interpolates between this point and another
    /// \param other - vector to interpolate with
    /// \param t - interpolant. Clamped to [0,1]
    /// \return coordinates of interpolated point. (other - *this) * t
    template <typename = IsFloatingPoint<T>>
    constexpr Vector2 Lerp(Vector2 other, T t)
    {
        t = std::clamp(t, static_cast<T>(0), static_cast<T>(1));
        return *this + (other - *this) * t;
    }

    /// Linearly interpolates between two points
    /// \param first - first point coordinates
    /// \param other - second point coordinates
    /// \param t - interpolant. Clamped to [0,1]
    /// \return coordinates of interpolated point. (second - first) * t
    template <typename = IsFloatingPoint<T>>
    static constexpr Vector2 Lerp(Vector2 first, Vector2 second, T t)
    {
        return first.Lerp(second, t);
    }

    /// Clamps magnitude of a vector to maxLength
    /// \param maxLength - maximum vector length
    template <typename = IsFloatingPoint<T>>
    constexpr void ClampMagnitude(T maxLength)
    {
        T oldLength = Length();

        if (Math::IsExtremelyClose(oldLength, static_cast<T>(0)))
        {
            return;
        }

        T length = std::clamp(oldLength, static_cast<T>(0), maxLength);

        *this /= oldLength;
        *this *= length;
    }

    /// Clamps vector's magnitude to maxLength
    /// \param v - vector to clamp
    /// \param maxLength - maximum vector length
    /// \return copy of v with its magnitude clamped to maxLength
    template <typename = IsFloatingPoint<T>>
    static constexpr Vector2 ClampMagnitude(Vector2 v, float maxLength)
    {
        v.ClampMagnitude(maxLength);
        return v;
    }

    /// Rotates a vector clockwise by angle
    /// \param angle - angle in radians
    template<typename = IsFloatingPoint<T>>
    void Rotate(T angle)
    {
        // I am god damn tired of static_casts and warnings. TODO: find a solution

        double dAngle = static_cast<double>(angle);
        // TODO: implement matrix class ?
        Vector2 temp = *this;
        x = temp.x * static_cast<T>(std::cos(dAngle)) - temp.y * static_cast<T>(std::sin(dAngle));
        y = temp.x * static_cast<T>(std::sin(dAngle)) + temp.y * static_cast<T>(std::cos(dAngle));
    }

    /// Get squared length
    /// \return squared length of the vector
    template<typename = IsFloatingPoint<T>>
    constexpr T SqrdLength() const
    {
        return x * x + y * y;
    }

    template<typename = IsFloatingPoint<T>>
    constexpr T Angle() const
    {
        return std::fmod(std::atan2(y, x) + 2 * Math::kPi, 2 * Math::kPi);
    }

    // TODO: add other operators

    /// += operator overload for Vector2
    constexpr Vector2& operator+= (const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr bool operator== (const Vector2& other)
    {
        if constexpr (std::is_floating_point_v<T>)
            return (Math::IsExtremelyClose(x, other.x)) && (Math::IsExtremelyClose(y, other.y));
        else
            return (x == other.x) && (y == other.y);
    }

    /// *= operator overload for Vector2
    constexpr Vector2& operator*= (T value)
    {
        x *= value;
        y *= value;
        return *this;
    }

    /// /= operator overload for Vector2
    constexpr Vector2& operator/= (T value)
    {
        x /= value;
        y /= value;
        return *this;
    }
};

/// Operator * overload. Multiply 2D vector by a number
template <typename T>
inline constexpr Vector2<T> operator* (const Vector2<T>& lhs, T rhs)
{
    return Vector2<T>(lhs.x * rhs, lhs.y * rhs);
}

/// Operator * overload. Multiply a number by a 2D vector
template <typename T>
inline constexpr Vector2<T> operator* (T lhs, const Vector2<T>& rhs)
{
    return rhs * lhs;
}
/// Operator * overload. Dot product of two 2D vectors
template <typename T>
inline constexpr T operator* (const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
/// Operator / overload. Divides 2D vector by a number
template <typename T>
inline constexpr Vector2<T> operator/ (const Vector2<T>& lhs, T rhs)
{
    return Vector2<T>(lhs.x / rhs, lhs.y / rhs);
}
/// Operator + overload. Adds two 2D vectors
template <typename T>
inline constexpr Vector2<T> operator+ (const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}
/// Operator + overload. Adds two 2D vectors of different types
template <typename T, typename U>
inline constexpr Vector2<T> operator+ (const Vector2<T>& lhs, const Vector2<U>& rhs)
{
    return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}
/// Operator - overload. Subtracts two 2D vectors
template <typename T>
inline constexpr Vector2<T> operator- (const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template <typename T>
inline constexpr Vector2<T> operator- (const Vector2<T>& other)
{
    return Vector2(-other.x, -other.y);
}

template<typename T>
inline constexpr Vector2<T>::Vector2(T _x, T _y)
    :x(_x)
    , y(_y)
{
}

template<typename T>
template<typename U>
inline constexpr Vector2<T>::Vector2(const Vector2<U>& other)
    : x(static_cast<T>(other.x))
    , y(static_cast<T>(other.y))
{
}

template<typename T>
inline bool Vector2<T>::IsInside(const Rectangle<T>& rect) const
{
    return rect.Contains(*this);
}

/// Alias for Vector2 of type int
using IVec2 = Vector2<i32>;
/// Alias for Vector2 of type float
using FVec2 = Vector2<f32>;
} // namespace yang