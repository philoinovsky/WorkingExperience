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
## V. Templates
### Parameterized Types
```
template<typename T>
```
### Concepts and Generic Programming
1. the ability to pass types
2. delayed type checking (happends at instantiation time)
3. ability to pass constant values as arguments, implying compile-time computation
### Function Objects
the class defined `()` operator
### Variadic Template
```
void f() {}
template<typename T, typename... Tail>
void f(T head, Tail... tail)
{
    g(head);
    f(tail...);
}
```
dont use it for homogeneous argument lists (prefer initializer lists)
## VII. IO Streams
### I/O of User-Defined Types
```
istream& operator >> (istream& is, Entry &e) {
    // ...
    // success: return is;
    // fail: is.setstate(ios_base::failbit);
    //      return is;
}
```
### stringstream
can be used to convert different types of objects
```
stringstream interpreter;
if (
    !(interpreter << arg)
    || !(interpreter >> result)
    || !(interpreter >> std::ws).eof()
) throw runtime_error("failed");
```
## VIII. Containers
1. `vector` faster than `list` (consecutive memory layout)
2. use `at()` when you need guaranteed range checks
3. use `forward_list` for sequence that are usually empty (optimized for empty sequence, just one word)
## IX. Algorithms
1. `back_inserter(res)` constructs an iterator for `res` that adds elements at the end of a container
```
unique_copy(vec.begin(), vec.end(), back_inserter(res))
```
