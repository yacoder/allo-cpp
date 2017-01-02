/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

namespace allo
{
namespace strategies
{

// Doesn't really allocate anything or track any pointers,
// just counts the number of calls to allocate & deallocate.
template <typename T> class counting_strategy
{
 public:
   T* allocate(std::size_t)
   {
      ++m_number_of_allocations;
      return nullptr;
   }

   bool deallocate(T*, std::size_t)
   {
      ++m_number_of_deallocations;
      return false;
   }

   using counter_t = uint32_t;

   counter_t number_of_allocations() const { return m_number_of_allocations; }
   counter_t number_of_deallocations() const { return m_number_of_deallocations; }

   counting_strategy() = default;
   counting_strategy(const counting_strategy&) = delete;
   counting_strategy(counting_strategy&&) = delete;
   counting_strategy& operator=(const counting_strategy&) = delete;
   counting_strategy& operator=(counting_strategy&&) = delete;
   ~counting_strategy() = default;

 private:
   counter_t m_number_of_allocations = 0;
   counter_t m_number_of_deallocations = 0;
};

} // namespace strategies
} // namespace allo
