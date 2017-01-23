Allo is a header-only library of fast composable C++ allocators with conveninent wrappers for a variety of containers.

Quick sample

```cpp
{
   // Create an allo::map and tell it how many bytes of memory to allocate for a private arena.
   // allo::map creates the arena and does all the wiring with std::map behind the scenes. 
   allo::map<int, int> allo_map(65536);
    
   // Get the std::map from within the allo::map.
   // Use auto&, because the type of it is NOT simply std::map<int, int>, it has a custom allocator,
   // but, naturally, it still "quacks like a map": you can call all the standard map methods on the reference.
   auto& map = allo_map.unwrap();
       
   // ... use the map here ...
       
   // At the end of scope allo::map deallocates the memory.
   // If the types in the map are trivially destructible, and there were no allocations outside
   // the private pool, allo::map will deallocate the pool as a whole in one operation without
   // destructing each node of the map's tree.
}
```

# Tested with
Visual Studio 2015 Update 3

# How to run tests
1. Run script from under extras\tests\scripts to generate VS 2015 solution from CMake
2. Open solution, build and run project "allo-tests"
3. Unit tests are implemented with [Catch](https://github.com/philsquared/Catch/blob/master/docs/tutorial.md)

# How to run benchmarks
1. Run script from under extras\benchmarks\scripts to generate VS 2015 solution from CMake
2. Open solution, build and run project "allo-benchmarks"

