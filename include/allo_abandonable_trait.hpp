/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once
#include<map>

namespace allo
{

// If a type is abandonable, allo-containers are allowed to not deallocate it,
// when they are destructed together with the private memory arena.
template <typename T> struct is_abandonable : std::conditional<std::is_trivially_destructible<T>::value, std::true_type, std::false_type>
{
};

template <typename TKey, typename TValue, typename TCompare, typename TAlloc>
struct is_abandonable<std::map<TKey, TValue, TCompare, TAlloc>>
    : std::conditional<is_abandonable<TKey>::type::value && is_abandonable<TValue>::type::value,
                       std::true_type, std::false_type>
{
};

} // namespace allo
