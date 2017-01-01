/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

namespace allo
{

// Actual counters for counting_allocator.
class counting_allocator_data final
{
 public:
   using counter_t = uint32_t;

   counter_t number_of_allocations() const { return m_number_of_allocations; }
   counter_t number_of_deallocations() const { return m_number_of_deallocations; }

 private:
   template <typename T> friend class counting_allocator;
   uint32_t m_number_of_allocations = 0;
   uint32_t m_number_of_deallocations = 0;
};

// Allocator, which counts the number of operations.
template <typename T> class counting_allocator
{
 public:
   // Allocator trait interface.

   using value_type = T;

   template <class U> counting_allocator(const counting_allocator<U>& other)
   {
      // just don't propagate data pointer in this case to avoid double accounting
      // MSVC uses this constructor to create a wrapper allocator in Debug build
   }

   counting_allocator(const counting_allocator& other) = default;

   T* allocate(std::size_t n)
   {
      if (m_data)
         ++m_data->m_number_of_allocations;

      return new T;
   }

   void deallocate(T* p, std::size_t n)
   {
      if (m_data)
         ++m_data->m_number_of_deallocations;

      delete p;
   }

   // Interface beyond Allocator trait.

   counting_allocator(counting_allocator_data* data)
       : m_data(data)
   {
   }

   counting_allocator(counting_allocator&&) = delete;
   counting_allocator& operator=(const counting_allocator&) = delete;
   counting_allocator& operator=(counting_allocator&&) = delete;

 private:
   counting_allocator_data* m_data = nullptr;
};

} // namespace allo
