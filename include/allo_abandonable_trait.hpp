/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

namespace allo
{

template <typename T> struct is_abandonable : std::false_type
{
};

template <typename TKey, typename TValue>
struct is_abandonable<std::map<TKey, TValue>>
    : std::conditional<std::is_trivially_destructible<TKey>::value && std::is_trivially_destructible<TValue>::value,
                       std::true_type, std::false_type>
{
};

} // namespace allo
