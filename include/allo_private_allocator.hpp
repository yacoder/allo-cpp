/*

Copyright (c) 2017 Maksim Galkin

This file is subject to the terms and conditions of the MIT license,
distributed with this code package in the LICENSE file, also available at:
https://github.com/yacoder/allo-cpp/blob/master/LICENSE

*/

#pragma once

namespace allo
{

// Our main building block - a composite extendable thread-local allocator.
// This class itself moslty just covers the Allocator trait implementation,
// while the allocation algorithm is encapsulated in TAllocationStrategy.
// This allocator is not thread-safe, intended for thread-local usage only.
template <typename T, typename TAllocationStrategy, typename TInnerAllocator = std::allocator<T>>
class private_allocator
{
 public:
   // Allocator trait interface.

   using value_type = T;

   template <class U> private_allocator(const private_allocator<U, TAllocationStrategy, TInnerAllocator>& other)
   {
      // e.g. MSVC calls this constructor to create a wrapper allocator in Debug build
   }

   T* allocate(std::size_t n)
   {
      T* t = m_allocation_strategy.allocate(n);

      if (!t)
         t = m_inner_allocator.allocate(n);

      return t;
   }

   void deallocate(T* p, std::size_t n)
   {
      if (!m_allocation_strategy.deallocate(p, n))
         m_inner_allocator.deallocate(p, n);
   }

   // Interface beyond Allocator trait.

   private_allocator(TAllocationStrategy& strategy, TInnerAllocator& inner_allocator)
       : m_allocation_strategy(strategy)
       , m_inner_allocator(inner_allocator)
   {
   }

   private_allocator(const private_allocator& other) = default;
   private_allocator(private_allocator&&) = delete;
   private_allocator& operator=(const private_allocator&) = delete;
   private_allocator& operator=(private_allocator&&) = delete;
   ~private_allocator() = default;

 private:
   TAllocationStrategy& m_allocation_strategy;
   TInnerAllocator& m_inner_allocator;
};

} // namespace allo
