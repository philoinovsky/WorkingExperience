# A Tour of C++
## I. the Basics
### `const` and `constexpr`
1. `const`: for variable, could be runtime constant/compile-time constant; for method, the return pointer is promised not to be modified, if return by value, no difference with normal function.
2. `constexpr`: for variable, promised to be computed at compile time; for method, could be compile-time and runtime. Advice: if a function may be evaluated at compile time, use `constexpr`
## III. Modularity
### `noexcept`
1. provide oppotunities for compiler to optimize, 
```
void foo() { A a; bar(a); }
```
compiler will produce code that once constructing `a` throws error, the destructor will be called. If `noexcept` such code will be optimized. This works for all functions being called by `noexcept` functions.
2. never use `noexcept`, except for:
- desctructor
- simple getter, setter
- move constructor/assignment-er
## IV. Class
### I. cast
1. `static_cast`: doesnt check, just cast
2. `dynamic_cast`: cast parent to child (it should be a child actually though)
### II. copy, move
1. copy: `const T&`
2. move: `T&&`
3. five situation:
- source of assignment
- object initializer
- function argument
- function return value
- exception
