/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

#include "../allo_abandon.hpp"
#include "../allo_abandonable_trait.hpp"

#include "../allo_never_look_back_allocator.hpp"
#include "../allocation_strategies/allo_never_look_back_strategy.hpp"

#include <vector>

namespace allo
{
namespace containers
{

template <template <typename TAlloc> class TContainer, typename TValue,
          template <typename T> class TFallbackAlloc>
class wrapped_container
{
 public:
   wrapped_container(size_t private_memory_size)
       : m_memory(private_memory_size)
       , m_strategy(m_memory.data(), m_memory.data() + m_memory.size())
       , m_allocator(m_strategy)
       , m_wrapped(m_allocator)
   {
   }

   template<typename UFallbackAlloc>
   wrapped_container(size_t private_memory_size, UFallbackAlloc&& fallback_allocator)
       : m_memory(private_memory_size)
       , m_strategy(m_memory.data(), m_memory.data() + m_memory.size())
       , m_allocator(m_strategy, std::forward<UFallbackAlloc>(fallback_allocator))
       , m_wrapped(m_allocator)
   {
   }

   wrapped_container(const wrapped_container&) = delete;
   wrapped_container(wrapped_container&&) = delete;
   wrapped_container& operator=(const wrapped_container&) = delete;
   wrapped_container& operator=(wrapped_container&&) = delete;

   using allocator_t = never_look_back_allocator<TValue, TFallbackAlloc<TValue>>;

   ~wrapped_container()
   {
      if (is_abandonable<TContainer<allocator_t>>::type::value && m_strategy.no_allocation_requests_have_failed())
      {
         abandon(m_wrapped);
      }
   }

   TContainer<allocator_t>& unwrap() { return m_wrapped; }
   const TContainer<allocator_t>& unwrap() const { return m_wrapped; }

 private:
   std::vector<uint8_t> m_memory;

   strategies::never_look_back_strategy m_strategy;

   allocator_t m_allocator;
   TContainer<allocator_t> m_wrapped;
};

} // namespace containers
} // namespace allo
