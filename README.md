Allo is a header-only library of fast composable C++ allocators with conveninent wrappers for a variety of containers.

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

It helps in several scenarios:
 - you need to use a short-lived container (e.g. find the most frequent number and return just the number)
 - you run this code in parallel and you need to reduce the contention over standard allocator
 - the types in the container are trivially destructible

# Is it stable?
It's in a pre-release v0.1-alpha stage, there are a few major gaps at the moment, e.g. custom allocators need to support proper memory alignment requirements. But it seems to work for a few simple cases covered by the tests I wrote so far. 

# Tested with
Visual Studio 2015 Update 3

# How to run tests
1. Run script from under extras\tests\scripts to generate VS 2015 solution from CMake
2. Open solution, build and run project "allo-tests"
3. Unit tests are implemented with [Catch](https://github.com/philsquared/Catch/blob/master/docs/tutorial.md)

# How to run benchmarks
1. Run script from under extras\benchmarks\scripts to generate VS 2015 solution from CMake
2. Open solution, build and run project "allo-benchmarks"

