/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "allo.hpp"

#include <vector>

TEST_CASE("Counting allocator works with standard vector", "[vector]")
{
   using allocator_t = allo::counting_allocator<int>;
   allo::counting_allocator_data counters;

   {
      std::vector<int, allocator_t> v(allocator_t{ &counters });
      v.push_back(1);

      REQUIRE(counters.number_of_allocations() == 1);
      REQUIRE(counters.number_of_deallocations() == 0);
   }

   REQUIRE(counters.number_of_allocations() == 1);
   REQUIRE(counters.number_of_deallocations() == 1);
}
