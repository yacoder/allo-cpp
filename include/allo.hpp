/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

// This is the root header file, which simply includes all sub-headers.
// Consumers of the library can include this header for simplicty, e.g. in
// their pre-compiled header, or include some of the sub-headers directly to
// speed up compilation.

#include "allo_private_allocator.hpp"

#include "allocation_strategies/allo_counting_strategy.hpp"
#include "allocation_strategies/allo_never_look_back_strategy.hpp"

#include "allo_counting_allocator.hpp"
#include "allo_never_look_back_allocator.hpp"
