/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "allo.hpp"

#include <array>
#include <vector>

TEST_CASE("Counting strategy can count", "[counting-strategy]")
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

TEST_CASE("Never look back strategy allocates until out of buffer", "[never-look-back-strategy]")
{
    std::array<uint8_t, 3> buffer{};
    auto* buffer_begin = buffer.data();
    auto* buffer_end = buffer.data() + buffer.size();
    allo::strategies::never_look_back_allocation_strategy<uint8_t> never_look_back_strategy(buffer_begin, buffer_end);

    auto* first_byte = never_look_back_strategy.allocate(1);
    REQUIRE(first_byte == buffer.data());

    auto* too_large = never_look_back_strategy.allocate(3);
    REQUIRE(too_large == nullptr);

    auto* last_two_bytes = never_look_back_strategy.allocate(2);
    REQUIRE(last_two_bytes == buffer.data() + 1);

    // should run out of buffer by now
    REQUIRE(never_look_back_strategy.allocate(1) == nullptr);

    // strategy must not recognize other pointers
    REQUIRE(never_look_back_strategy.deallocate(nullptr, 1) == false);
    REQUIRE(never_look_back_strategy.deallocate(buffer_end, 1) == false);

    // strategy must recognize the pointers it allocated
    REQUIRE(never_look_back_strategy.deallocate(first_byte, 1) == true);
    REQUIRE(never_look_back_strategy.deallocate(last_two_bytes, 2) == true);

    // even after we have deallocated everything, we can't allocate again fom it
    REQUIRE(never_look_back_strategy.allocate(1) == nullptr);
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
