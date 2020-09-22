#pragma once

/// \file Rectangle.h
/// Rectangle class description

#include "Math.h"
#include "Vector2.h"

//! namespace yang Contains all Yangine code
namespace yang
{

/// \class Rectangle
/// Represents the rectangle
template <typename T>
class Rectangle
{
    static_assert(std::is_arithmetic_v<T>, "Rectangle template param should be arithmetic");
public:
    T x;        ///< X coordinate of top left corner
    T y;        ///< Y coordinate of top left corner
    T width;    ///< height of the rectangle
    T height;   ///< width of the rectangle

    /// Default constructor
    Rectangle() = default;

    /// Constructor
    /// \param x - X coordinate of top left corner
    /// \param y - Y coordinate of top left corner
    /// \param width - width of the rectangle
    /// \param height - height of the rectangle
    Rectangle(T x, T y, T width, T height);

    /// Constructor
    /// \param center - coordinates of rectangle center
    /// \param dimensions - width and height of the rectangle
    Rectangle(const Vector2<T>& center, const Vector2<T>& dimensions);

    /// Explicitly defaulted copy constructor
    Rectangle(const Rectangle&) = default;

    /// Explicitly defaulted copy assignment operator
    Rectangle& operator=(const Rectangle&) = default;

    /// Default destructor
    ~Rectangle() = default;

    /// Determines whether two rectangles collide
    /// \param other - rectangle to check for collision with
    /// \return true if rectangles collide
    bool Collide(const Rectangle& other) const;

    /// Determines whether rectangle contains the point
    /// \param point - point to check
    /// \return true if the point is inside of the rectangle
    bool Contains(const Vector2<T>& point) const;

    /// Constructor that converts Rectangle of other type to Rectangle of this type
    /// \tparam U - type to convert to
    /// \param other - rectangle to convert
    template <typename U>
    Rectangle<T>(const Rectangle<U>& other);
};

template<typename T>
inline Rectangle<T>::Rectangle(T x, T y, T width, T height)
    :x(x)
    , y(y)
    , width(width)
    , height(height)
{
}

template<typename T>
inline Rectangle<T>::Rectangle(const Vector2<T>& center, const Vector2<T>& dimensions)
    :x(center.x - dimensions.x / 2)
    , width(dimensions.x)
    , y(center.y - dimensions.y / 2)
    , height(dimensions.y)
{
}

template<typename T>
inline bool Rectangle<T>::Collide(const Rectangle& other) const
{
    bool collisionX = x < other.x + other.width && x + width > other.x;
    bool collisionY = y < other.y + other.height && y + height > other.y;
    return collisionX && collisionY;
}

template<typename T>
inline bool Rectangle<T>::Contains(const Vector2<T>& point) const
{
    return (point.x > x) && (point.x < x + width) && (point.y > y) && (point.y < y + height);
}

template <typename T>
template <typename U>
inline Rectangle<T>::Rectangle(const Rectangle<U>& other)
    :x(static_cast<T>(other.x))
    , y(static_cast<T>(other.y))
    , width(static_cast<T>(other.width))
    , height(static_cast<T>(other.height))
{
}

/// Alias for Rectangle of type float
using FRect = Rectangle<f32>;
/// Alias for Rectangle of type int
using IRect = Rectangle<int>;

} // namespace yang