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

template <template <typename TAlloc> typename TContainer, typename TValue> class wrapped_container
{
 public:
   wrapped_container(size_t private_memory_size)
       : m_memory(private_memory_size)
       , m_strategy(m_memory.data(), m_memory.data() + m_memory.size())
       , m_allocator(m_strategy, m_std_allocator)
       , m_wrapped(m_allocator)
   {
   }

   using allocator_t = never_look_back_allocator<TValue, std::allocator<TValue>>;

   ~wrapped_container()
   {
      if (is_abandonable<allocator_t>::value)
      {
         abandon(m_wrapped);
      }
   }

   TContainer<allocator_t>& unwrap() { return m_wrapped; }
   const TContainer<allocator_t>& unwrap() const { return m_wrapped; }

 private:
   std::vector<uint8_t> m_memory;
   std::allocator<TValue> m_std_allocator;

   strategies::never_look_back_strategy<TValue> m_strategy;

   
   allocator_t m_allocator;
   TContainer<allocator_t> m_wrapped;
};

} // namespace containers
} // namespace allo
