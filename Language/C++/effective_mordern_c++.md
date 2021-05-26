# Effective Mordern C++
## Chapter 1: Deducing Types
### I. Understand Template Type Deduction
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
### II. Understand `auto` type deduction
1. same as template type deduction, except that `auto` regards `initializer_list<>` as it is, while template type deduction does not.
2. when use `auto` as function return types or lambda arguments, `auto` implies template type deduction.
### III. Understand `decltype`
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
### IV. Know how to view deduced types
1. IDE - not precise sometimes
2. boost.typeindex
## Chapter 2: auto
### V. prefer `auto` to explicit type declarations
- TODO
### VI. use the explicitly typed initializer idiom when `auto` deduces undesired types
- TODO
## Chapter 3: Moving to Modern C++
### VII. distinguish between `()` and `{}` when creating objects
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
### VIII. Prefer `nullptr` to `0` and `NULL`
avoid overloading on integral and pointer types.
### IX. Prefer alias declarations (`using`) to `typedef`
#### ToR
1. `typedef`s dont support templatization, but alias declaration do
2. alias templates avoid the "::type" suffix and, in templates, the "typename" prefix often required to refer to `typedef`s
3. C++14 offers alias templates for all the C++11 type traits transformation
### X. Prefer scoped enums to unscoped enums
- scoped enum: `enum class Color {black, white, red}`
- unscoped enum: `enum Color {black, white, red}`
#### ToR
1. both scoped and unscoped `enums` support specification of the underlying type. The default underlying type for scoped `enum`s is `int`. Unscoped have no default undelying type.
2. scoped `enum`s may always be forward-declared. Unscoped `enum`s may be forward-declared only if their declaration specifies an underlying type.
### XI. Prefer deleted functions to private undefined ones
any function may be deleted, including non-member functions and template instantiations
### XII. Declare overriding functions `override`
member function reference qualifiers make it possible to treat lvalue and rvalue objects (*this) differently
### XIII. Prefer `const_iterators` to `iterator`
- TODO
