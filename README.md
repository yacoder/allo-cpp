Allo is a header-only library of fast composable C++ allocators with conveninent wrappers for a variety of containers.

[![Build Status](https://travis-ci.org/yacoder/allo-cpp.svg?branch=master)](https://travis-ci.org/yacoder/allo-cpp)

# Quick sample

```cpp
{
   // Create an allo::map and tell it how many bytes of memory to allocate for a private arena.
   // allo::map creates the arena and does all the wiring with std::map behind the scenes. 
   allo::map<int, int> allo_map(65536);
    
   // Get the std::map from within the allo::map.
   // Use auto&, because the type of it is NOT simply std::map<int, int>, it has a custom allocator,
   // but, naturally, it "quacks like a map": you can call all the standard map methods on the reference.
   auto& map = allo_map.unwrap();
       
   // ... use the map here ...
       
   // At the end of scope allo::map deallocates the memory.
   // If the types in the map are trivially destructible, and there were no allocations outside
   // the private pool, allo::map will deallocate the pool as a whole in one operation without
   // destructing each node of the map's tree.
}
```
# Is it faster?

Yes, the table below shows the result of the *most_frequent_numbers* benchmark on my 2-core i7 laptop, built with VS 2015.3. It runs the same algorithm on multiple threads, the difference is only in the container type used.

Number of threads | `std::map` time, ms | `allo::map` time, ms
--- | --- | ---
1|13779|9487
2|8149|5222
3|6293|3974
4|5529|3320

Generally speaking, this approach helps in algortihms, which use a short-lived temporary container (e.g. find the most frequent number and return just the number) and in addition to that:
 - you might run this algorithms in parallel and you want to reduce contention over process heap
 - the types in the temporary container are trivially destructible and you want to speed-up the destruction

# Is it stable?
It's in a pre-release v0.1-alpha stage, there are a few major gaps at the moment, e.g. custom allocators need to support proper memory alignment requirements. But it seems to work for a few simple cases covered by the tests written so far. 

# Builds with
- Visual Studio 2015 Update 3
- GCC 5, Linux
- Clang 3.7, Linux
- Apple LLVM version 8.0.0 (clang-800.0.42.1), OS X

# How to run tests
1. Run script from under extras\tests\scripts to generate VS 2015 solution from CMake
2. Open solution, build and run project "allo-tests"
3. Unit tests are implemented with [Catch](https://github.com/philsquared/Catch/blob/master/docs/tutorial.md)

# How to run benchmarks
1. Run script from under extras\benchmarks\scripts to generate VS 2015 solution from CMake
2. Open solution, build and run project "allo-benchmarks"

# Related work
This library builds on the approach discussed in the [cup-of-t screencast episode 3](https://youtu.be/4URfka1mBuU), the list of references for that episode is here: https://github.com/yacoder/cup-of-t/blob/master/cup0003-cpp-stateful-allocators/notes.md  
