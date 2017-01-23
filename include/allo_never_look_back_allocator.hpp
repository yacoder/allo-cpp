/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

#include "allo_private_allocator.hpp"
#include "allocation_strategies/allo_never_look_back_strategy.hpp"

namespace allo
{

template <typename T, typename TInnerAllocator = std::allocator>
using never_look_back_allocator = private_allocator<T, strategies::never_look_back_strategy, TInnerAllocator>;

} // namespace allo
