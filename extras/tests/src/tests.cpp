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

TEST_CASE("Counting stratgy can count", "[counting-strategy]")
{
    allo::strategies::counting_strategy<void> counting_strategy;

    REQUIRE(counting_strategy.number_of_allocations() == 0);
    REQUIRE(counting_strategy.number_of_deallocations() == 0);

    REQUIRE(counting_strategy.allocate(100) == nullptr);

    REQUIRE(counting_strategy.number_of_allocations() == 1);
    REQUIRE(counting_strategy.number_of_deallocations() == 0);

    REQUIRE(counting_strategy.deallocate(&counting_strategy, 1) == false);

    REQUIRE(counting_strategy.number_of_allocations() == 1);
    REQUIRE(counting_strategy.number_of_deallocations() == 1);
}

TEST_CASE("Counting allocator works with standard vector", "[counting-allocator][counting-strategy][vector]")
{
   using allocator_t = allo::counting_allocator<int>;
   allo::strategies::counting_strategy<int> counting_strategy;

   {
      std::allocator<int> std_allocator;
      allocator_t allocator(counting_strategy, std_allocator);
      std::vector<int, allocator_t> v(allocator);
      v.push_back(1);

      REQUIRE(counting_strategy.number_of_allocations() == 1);
      REQUIRE(counting_strategy.number_of_deallocations() == 0);
   }

   REQUIRE(counting_strategy.number_of_allocations() == 1);
   REQUIRE(counting_strategy.number_of_deallocations() == 1);
}
