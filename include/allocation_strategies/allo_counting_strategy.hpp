/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once
#include <memory>

namespace allo
{
namespace strategies
{

// Doesn't really allocate anything or track any pointers,
// just counts the number of calls to allocate & deallocate.
class counting_strategy
{
 public:
   template <typename T> T* allocate(std::size_t)
   {
      ++*m_number_of_allocations;
      return nullptr;
   }

   template <typename T> bool deallocate(T*, std::size_t)
   {
      ++*m_number_of_deallocations;
      return false;
   }

   using counter_t = uint32_t;

   counter_t number_of_allocations() const { return *m_number_of_allocations; }
   counter_t number_of_deallocations() const { return *m_number_of_deallocations; }

   counting_strategy()
       : m_number_of_allocations(std::make_shared<counter_t>(0))
       , m_number_of_deallocations(std::make_shared<counter_t>(0))
   {
   }

   counting_strategy(const counting_strategy&) = default;
   ~counting_strategy() = default;

   counting_strategy(counting_strategy&&) = delete;
   counting_strategy& operator=(const counting_strategy&) = delete;
   counting_strategy& operator=(counting_strategy&&) = delete;

 private:
   std::shared_ptr<counter_t> m_number_of_allocations;
   std::shared_ptr<counter_t> m_number_of_deallocations;
};

} // namespace strategies
} // namespace allo
