/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

#include "allo_wrapped_container.hpp"

#include <map>
#include <memory>
#include <utility>

namespace allo
{
namespace containers
{

template <typename TKey, typename TValue> struct allo_map
{
   template <typename TAlloc> using type = std::map<TKey, TValue, std::less<TKey>, TAlloc>;
};

template <typename TKey, typename TValue, template <typename T> class TFallbackAlloc = std::allocator>
using wrapped_map =
    wrapped_container<typename allo_map<TKey, TValue>::type, std::pair<const TKey, TValue>, TFallbackAlloc>;

} // namespace containers
} // namespace allo
