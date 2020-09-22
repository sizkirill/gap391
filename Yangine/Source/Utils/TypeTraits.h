#pragma once
/// \file TypeTraits.h
/// Type traits extension for Yangine
#include "Math.h"
#include "Vector2.h"
#include <memory>
#include <optional>

//! namespace yang Contains all Yangine code
namespace yang
{

/// Helper struct to determine whether a type is Vector2<T> or not
template <class T>
struct is_vec2 : std::false_type
{};
/// Helper struct to determine whether a type is Vector2<T> or not
template <class T>
struct is_vec2<yang::Vector2<T>> : std::true_type
{};
/// Bool struct to determine whether a type is Vector2<T> or not
template <class T>
inline constexpr bool is_vec2_v = is_vec2<T>::value;

/// Helper struct to determine whether a type is std::shared_ptr<T> or not
template <class T>
struct is_shared_ptr : std::false_type
{};
/// Helper struct to determine whether a type is std::shared_ptr<T> or not
template <class T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
{};
/// Bool struct to determine whether a type is std::shared_ptr<T> or not
template <class T>
inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

/// Helper struct to determine whether a type is std::optional<T> or not
template <class T>
struct is_optional : std::false_type
{};
/// Helper struct to determine whether a type is std::optional<T> or not
template <class T>
struct is_optional<std::optional<T>> : std::true_type
{};
/// Bool to determine whether a type is std::optional<T> or not
template <class T>
inline constexpr bool is_optional_v = is_optional<T>::value;

}