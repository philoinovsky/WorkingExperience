# Effective Mordern C++
## Chapter 1: Deducing Types
### 1. Understand Template Type Deduction
passed param type -> T, param type in function
```
template<typename T>
void f(??? param)
```
#### case 1: reference or a pointer, but not a universal reference
T& param
```
int -> int, int&
const int -> const int, const int &
const int & -> const int, const int &
```
const T& param
```
int -> int, const int &
const int -> int, const int &
const int & -> int, const int &
```
#### case 2: universal reference
T&& param
```
lvalue int -> int &, int &
lvalue const int -> const int &, const int &
lvalue const int & -> const int &, const int &
rvalue int -> int, int&&
```
#### case 3: neither a pointer nor a reference (by-value)
T param
```
int -> int, int
const int -> int, int
const int & -> int, int

const int* const -> const int*, const int* (pointer will be copied bit by bit)
```
#### array arguments
allows `reference` to arrays by `T& param`, for normal functions, array can only be passed by decaying to a pointer. `T (&)[N]` can deduce the size of array. With `constexpr`, one can define `arraySize` function to declare arrays with same length.
#### function arguments
same as array, normal pass as decayed funciton poiter, but can pass by reference to `T& param`
#### ToR
1. during template type deductions, argument references are treated as non-references
2. when deducing types for a universal reference parameters, lvalue arguments get special treatment
3. when deducing types for `by-value` parameters, `const` and/or `volatile` arguments are treated as `non-const` and `non-volatile`
4. during template type deduction, arguments that are array or function names decay to pointers, unless they're used to initialize references.
### 2. Understand `auto` type deduction
1. same as template type deduction, except that `auto` regards `initializer_list<>` as it is, while template type deduction does not.
2. when use `auto` as function return types or lambda arguments, `auto` implies template type deduction.
### 3. Understand `decltype`
1. `decltype` typically parrots back the exact type of the name or expression you give it.
2. primary use of `decltype` is declaring function templates where the function's return type depends on its parameter types.
3. trailing return type
```
auto f(...) -> decltype(c[i]) // returns c[i]'s type // c++ 11
```
4. c++14
```
template<...>
decltype(auto) f(...)
```
5. support both lvalue and rvalue arguments by using `forward<T>`
#### ToR
1. for lvalue expressions of type T other than names, `decltype` always reports a type of `T&`
### 4. Know how to view deduced types
1. IDE - not precise sometimes
2. boost.typeindex
## Chapter 2: auto
### 5. prefer `auto` to explicit type declarations
- TODO
### 6. use the explicitly typed initializer idiom when `auto` deduces undesired types
- TODO
## Chapter 3: Moving to Modern C++
### 7. distinguish between `()` and `{}` when creating objects
```
int x(0); // parentheses
int y = 0; // =
int z{0}; int z = {0}; // initializer using braces
```
1. uncopiable objects cannot be initialized with `=`
2. cannot use `()` in class definition
3. `{}` is called `uniform` because only it can be used everywhere.
#### ToR
1. braced initialization is the most widely usable. And it's immune to C++'s most vexing parse. (i.e. `Widget w2()` is function or initialization)
2. during constructor overload, `{}` are matched to `std::initializer_list` parameters if at all possible, even if other constructors offer seemingly better matches
3. choosing between `()` and `{}` for object creation inside templates can be challenging
### 8. Prefer `nullptr` to `0` and `NULL`
avoid overloading on integral and pointer types.
### 9. Prefer alias declarations (`using`) to `typedef`
#### ToR
1. `typedef`s dont support templatization, but alias declaration do
2. alias templates avoid the "::type" suffix and, in templates, the "typename" prefix often required to refer to `typedef`s
3. C++14 offers alias templates for all the C++11 type traits transformation
### 10. Prefer scoped enums to unscoped enums
- scoped enum: `enum class Color {black, white, red}`
- unscoped enum: `enum Color {black, white, red}`
#### ToR
1. both scoped and unscoped `enums` support specification of the underlying type. The default underlying type for scoped `enum`s is `int`. Unscoped have no default undelying type.
2. scoped `enum`s may always be forward-declared. Unscoped `enum`s may be forward-declared only if their declaration specifies an underlying type.
### 11. Prefer deleted functions to private undefined ones
any function may be deleted, including non-member functions and template instantiations
### 12. Declare overriding functions `override`
member function reference qualifiers make it possible to treat lvalue and rvalue objects (*this) differently
### 13. Prefer `const_iterators` to `iterator`
- in maximally generic code, prefer non-member versions of `begin`, `end`, `rbegin`, etc., over their member function counterparts.
### 14. Declare functions `noexcept` if they won't emit exceptions
1. `noexcept` is part of a function's interface, and that means that callers may depend on it.
2. `noexcept` functions are more optimizable
3. `noexcept` is particularly valuable for the move operations, swap, memory deallocation functions, and destructors.
4. most functions are exception-neutral rather than `noexcept`
### 15. Use `constexpr` whenever possible
1. value known during compilation are privileged. They may be placed in read-only memory
2. `constexpr` objects are `const` and are initialized with values known during compilation
3. `constexpr` functions can produce compile-time results when called with argumentss whose values are known during compilation
4. `constexpr` objects and functions may be used in a wider range of contexts than `non-constexpr` objects and functions
5. `constexpr` is part of an object's or function's interface
### 16. Make `const` member functions thread safe
1. `std::mutex` is a `move-only type`
2. make `const` member functions thread safe unless you're `certain` they'll never be used in a concurrent context
3. `std::atomic` may have better performance, but only atomic to one variable/memory location
### 17. Understand special member function generation
#### A. Special Functions - which compilers may generate on their own
1. default constructor
2. destructor
3. copy operations
4. move operations
#### B. Generation
1. `move constructor and assignment op` generated if no `move operations`, `copy operations`, `destructor`
2. `copy constructor` generated if no `copy constructor`, deleted if a `move operation` is declared
3. `copy assignment` generated if no `copy assignment`, deleted if a `move operation` is declared
4. generation of the copy operations in classes with an explicitly declared destructor is deprecated
5. member function templates never suppress generation of special member functions
6. default constructor: generated if no user-declared
7. destructor: `noexcept` by default
## Chapter 4: Smart Pointers
### 18. Use `std::unique_ptr` for exclusive-ownership resource management
