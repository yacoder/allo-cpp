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

// Onwards and upwards!
// This strategy moves a pointer along the given memory area only forward,
// allocating consecutive elements for each request. Once the pointer
// have reached the end, no more allocations are possible.
// Deallocations will recognize allocated pointers, but will not
// actually reclaim any memory (no free list, or such).
//
// The simplest strategy, but still can be useful, for example if you
// populate a container just once upfront and then never change it later.
class never_look_back_strategy
{
 public:
   template <typename T> T* allocate(std::size_t n)
   {
      // TODO: use proper alignment
      auto* new_current_pointer = *m_current_pointer + sizeof(T) * n;
      if (new_current_pointer <= m_end_pointer)
      {
         T* result = reinterpret_cast<T*>(*m_current_pointer);
         *m_current_pointer = new_current_pointer;
         return result;
      }

      m_at_least_one_allocation_failed = true;
      return nullptr;
   }

   template <typename T> bool deallocate(T* p, std::size_t n)
   {
      const auto* pByte = reinterpret_cast<uint8_t*>(p);
      if (m_begin_pointer <= pByte && pByte < m_end_pointer)
      {
         // Pretend that we have "deallocated".
         // We need to do this to avoid composite allocator from
         // trying to deallocate with inner deallocator.
         return true;
      }

      return false;
   }

   never_look_back_strategy(uint8_t* begin, uint8_t* end)
       : m_begin_pointer(begin)
       , m_end_pointer(end)
       , m_current_pointer(std::make_shared<uint8_t*>(begin))
   {
   }

   never_look_back_strategy(const never_look_back_strategy&) = default;
   ~never_look_back_strategy() = default;

   never_look_back_strategy(never_look_back_strategy&&) = delete;
   never_look_back_strategy& operator=(const never_look_back_strategy&) = delete;
   never_look_back_strategy& operator=(never_look_back_strategy&&) = delete;

   bool no_allocation_requests_have_failed()
   {
       return !m_at_least_one_allocation_failed;
   }

 private:
   const uint8_t* m_begin_pointer;
   const uint8_t* m_end_pointer;
   std::shared_ptr<uint8_t*> m_current_pointer;
   bool m_at_least_one_allocation_failed = false;
};

} // namespace strategies
} // namespace allo
