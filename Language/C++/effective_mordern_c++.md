# Effective Mordern C++
## Deducing Types
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

