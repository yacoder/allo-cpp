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

// Our main building block - a composite extendable thread-local allocator.
// This class itself moslty just covers the Allocator trait implementation,
// while the allocation algorithm is encapsulated in TAllocationStrategy.
// This allocator is not thread-safe, intended for thread-local usage only.
template <typename T, typename TAllocationStrategy, typename TInnerAllocator = std::allocator<T>>
class private_allocator
{
 public:
   // ------------------------------------------------------------------------------------------
   // Allocator trait interface.

   using value_type = T;

   // Used by containters, e.g. to remap value_type allocator to node_type allocator.
   // See also: http://en.cppreference.com/w/cpp/memory/allocator
   template <typename U> struct rebind
   {
      using other = private_allocator<U, TAllocationStrategy,
                                      typename std::allocator_traits<TInnerAllocator>::template rebind_alloc<U>>;
   };

   // All private allocators are friends!
   template <typename U, typename UAllocationStrategy, typename UInnerAllocator> friend class private_allocator;

   template <typename U, typename UInnerAllocator>
   private_allocator(const private_allocator<U, TAllocationStrategy, UInnerAllocator>& other)
       : m_allocation_strategy(other.m_allocation_strategy)
       , m_inner_allocator(other.m_inner_allocator)
   {
      // MSVC calls this constructor to create a wrapper allocator in Debug build,
      // also used by containers to remap value_type allocator to node_type allocator.
   }

   T* allocate(std::size_t n)
   {
      T* t = m_allocation_strategy.template allocate<T>(n);

      if (!t)
         t = m_inner_allocator.allocate(n);

      return t;
   }

   void deallocate(T* p, std::size_t n)
   {
      if (!m_allocation_strategy.template deallocate<T>(p, n))
         m_inner_allocator.deallocate(p, n);
   }

   // ------------------------------------------------------------------------------------------
   // Interface beyond Allocator trait.

   private_allocator(TAllocationStrategy& strategy)
       : m_allocation_strategy(strategy)
   {
   }

   template <typename UInnerAllocator>
   private_allocator(TAllocationStrategy& strategy, UInnerAllocator&& innerAllocator)
       : m_allocation_strategy(strategy)
       , m_inner_allocator(std::forward<UInnerAllocator>(innerAllocator))
   {
   }

   private_allocator(const private_allocator& other) = default;
   private_allocator(private_allocator&&) = default;
   ~private_allocator() = default;

   private_allocator& operator=(const private_allocator&) = delete;
   private_allocator& operator=(private_allocator&&) = delete;

 private:
   TAllocationStrategy& m_allocation_strategy;
   TInnerAllocator m_inner_allocator;
};

} // namespace allo
