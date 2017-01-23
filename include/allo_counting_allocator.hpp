/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

#include "allo_private_allocator.hpp"
#include "allocation_strategies/allo_counting_strategy.hpp"

namespace allo
{

template<typename T> using counting_allocator = private_allocator<T, strategies::counting_strategy>;

} // namespace allo
