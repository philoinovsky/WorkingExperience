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
1. `unique_ptr` is a small, fast, move-only smart pointer for managing resources with exclusive-ownership semantics
2. by default, resource destruction takes place via `delete`, but custom deleters can be specified. Stateful deleters and function pointers as deleters increase the size of `std::unique_ptr` objects
3. converting a `std::unique_ptr` to a `std::shared_ptr` is easy
### 19. Use `std::shared_ptr` for shared-ownership resource management
1. `std::shared_ptr`s offers convenience approaching that of garbage collection for the shared lifetime management of arbitrary resources
2. `std::shared_ptr` are typically twice as big (count + ptr), incur overhead for control blocks, and require atomic reference count manipulations
3. default resource destruction is via `delete`, but custom deleters are supported. The type of the deleter has no effect on the type of the `std::shared_ptr`
#### A. Control Block
1. `make_shared` creates the control block and pointer at the same allocation, but the `weak_ptr` may prevent the control from being deleted
2. a control block is created when a `std::shared_ptr` is constructed from a unique-ownership pointer (`unique_ptr` or `auto_ptr`) 
3. when a `shared_ptr` constructor is called with a raw pointer, it creates a control block
4. if you want to create a `shared_ptr` from an object that already had a control block, you'd presumably pass a `shared_ptr` or a `weak_ptr` as a constructor argument, not a raw pointer.
5. if one created multiple control block from one raw pointer, they will release the resource multiple times, which leads to UB.
### 20. Use `std::weak_ptr` for `std::shared_ptr`-like pointers that can dangle
1. `weak_ptr` is an augmentation of `shared_ptr`, but it wont increase the count.
2. potential use cases for `std::weak_ptr` include caching (`unordered_map<ID, weak_ptr>`), observer lists (notify observer, but not interested in observer's lifetime), and the prevention of `std::shared_ptr` cycles (cyclic dependencies so all shared pointers won't be released).
3. use `weak_ptr::expired()` to check if it's dangling
4. `weak_ptr::lock()` returns `shared_ptr` if not dangling, returns null otherwise.
### 21. Prefer `std::make_unique` and `std::make_shared` to direct use of new
1. `shared_ptr(new xx)` consists of two steps: a. new object; b. create shared pointer, which may cause resource leak (if reordered and have exception in between) if directly use in function argument
2. `make_shared` make shared pointer in one step
3. `make` functions eliminate source code duplication, improve exception safety, and, for `std::make_shared` and `std::allocate_shared`, generate code that's smaller and faster
4. situations where use of `make` functions is inappropriate include the need to specify custom deleters and a desire to pass braced initializers.
5. for `std::shared_ptr`s, additional situations where `make` functinos may be ill-advised include a. classses with custom memory management and b. systems with memory concerns, very large objects, and `std::weak_ptr`s that outlive the corresponding `std::shared_ptr`s
### 22. When using the `Pimpl` idiom, define special member functions in the implementation file
1. the pimpl idiom decreases build times by reducing compilation dependencies between class clients and class implementations
2. for `std::unique_ptr` `pImpl` pointers, declare special member functions in the class header, but implement them in the implementation file. Do this even if the default function implementations are acceptable
3. the above advice applies to `std::unique_ptr`, but not to `std::shared_ptr`, which stems from the differing ways these smart pointers support custom deleters.
## Chapter 5: Rvalue References, Move Semantics, and Perfect Forwarding
### 23. Understand `std::move` and `std::forward`
1. `std::move` performs an unconditional cast to an rvalue. In and of itself, it doesnt move anything
2. `std::forward` casts its argument to an rvalue only if that argument is bound to an rvalue
3. neither `std::move` nor `std::forward` do anything at run time
4. all function parameters are lvalue. `forward` forwards based on the information encoded in `T`
### 24. Distinguish universal references from rvalue references
1. it must be precisely `T&&`
2. if a function template parameter has type `T&&` for a deduced type `T`, or if an object is declared using `auto&&`, the parameter or object is a universal reference
3. if the form of the type declaration isn't precisely `type&&`, or if type deduction does not occur, `type&&` denotes an rvalue reference
4. universal references correspond to rvalue references if they are initialized with rvalues. They correspond to lvalue references if they're initialized with lvalues.
### 25. use `std::move` on rvalue references, `std::forward` on universal references
1. apply `std::move` to rvalue references and `std::forward` to universal references the last time each is used.
2. do the same thing for rvalue references and universal references being returned from functions that return by value.
3. never apply `std::move` or `std::forward` to local objects if they would otherwise be pligible for the return value optimization.
### 26. Avoid overloading on universal references
1. overloading on universal references almost always leads to the universal reference overload being called more frequently than expected
2. perfect-forwarding constructors are especially problematic, because they're typically better matches than copy constructors for non-const lvalues, and they can hijack derived class calls to base class copy and move constructors
### 27. Familiarize yourself with alternatives to overloading on universal references
#### A. Abandon overloading
create functions for different types
#### B. Pass by `const T&`
#### C. Pass by value
can dial up performance without any increase in complexity
#### D. Use Tag dispatch
more parameters to make desired function matchable
#### E. Constraining templates that take universal references
1. `enabled_if_t` to conditionally enable the template initialization
2. `is_base_of::value` to enable template initialization for all subclass
3. maybe use `is_integral` or anything you need to partially disable the template initialization
4. use `decat_t` to remove references and `const` `volatile`
5. SFINAE
#### F. Trade-offs
1. ABC specify a type for each parameter in the function to be called
2. DE use perfect forwarding, hence dont specify types for the parameters. 
3. This fundamental decision - to specify a type or not - has consequences
### ToR
1. alternatives to the combination of universal references and overloading include the use of distinct function names, pass parameters by lvalue-reference-to-const, passing parameters by value, and using tag dispatch
2. constraining templates via `std::enable_if` permits the use of universal references and overloading together, but it controls the conditions under which compilers may use the universal reference overloads
3. universal reference parameters often have efficiency advantages, but they typically have usability disadvantages.
### 28. Understand reference collapsing
only && and && makes &&, the rest makes &
### 29. Assume that move operations are not present , not cheap, and not used
1. assume that move operations are not present, not cheap, and not used (e.g. when you writing templates, better be conservative)
2. in code with known types or support for move semantics, there is no need for assumptions
### 30. Familiarize yourself with perfect forwarding failure cases
1. braced initializers: then calling `fwd({1, 2, 3})`, the `{1, 2, 3}` is not declared to be a `initializer_list`, so it is a `non_deduced context` in template initialization
2. `0` or `NULL` as null pointers -> will be recognized as integral
3. Declaration-only integral `static const` data members: when `static const` member variable is declared but not defined, compiler wont allocate memory for it. So during function template initialization, it cannot be referred (reference and pointer use same mechanism). It can be solved by creating definition in `.cpp` file, but which cannot repeat the initializer.
4. Overloaded function names and template names: when functions are overloaded, compiler doesn't know which one you want to forward, doesnt know which template instance you want to forward.
5. Bitfields: since it's arbitrary bit range, it cannot be referenced. But you can fix this by passing only the integral value.
## Chapter 6: Lambda Expressions
### 31. Avoid Default Capture Modes
1. default by-reference capture can lead to dangling references (i.e. lambda outlives the referenced variable)
2. default by-value capture is susceptible to dangling pointer (especially `this`), and it misleadingly suggests that lambdas are self-contained
3. `static` members cannot be captures, they are references inside lambda.
### 32. Use init capture to move objects into closures
1. use C++14's init capture to move objects into closures
2. in C++11, emulate init capture via hand-written classes or `std::bind`
### 33. Use `decltype` on `auto&&` parameters to `std::forward` them
### 34. Prefer lambdas to `std::bind`
1. lambdas are more readable, more expensive, and may be more efficient than using `std::bind`
2. in C++11 only, `std::bind` may be useful for implementing move capture or for binding objects with templatized function call operators
## Chapter 7: The Concurrency API
### 35. Prefer task-based programming to thread-based
#### A. cases to use thread-based programming
1. you need access to the API of the underlying threading implementation
2. you need to and are able to optimize thread usage for your application
3. you need to implement threading technology beyond the C++ concurrency API
#### B. oversubscription
1. happends when there are more ready-to-run software threads than hardware threads.
2. when software thread is scheduled on a different core than last time it was, a. CPU caches are cold, b. it pollutes the cache for old threads
#### C. Advantage of task
1. state-of-the-art thread schedulers employ system-wide thread pools to avoid over-subscription, and they improve load lbalancing across hardware cores through work-stealing algorithm. 
2. if you take a task=based approach you already reap the benefits
#### ToR
1. `std::thread` cannot get result
2. thread-based programming calls for manual management of thread exhaustion, oversubscription, load balancing, and adaptation to new platforms.
3. task-based programming via `std::async` with the default launch policy handles most of these issues for you
### 36. Specify `std::launch::async` if asynchronnicity is essential
#### A. async policies
1. `std::launch::async`: the true async
2. `std::launch::deferred`: runs syncly only when `get` or `wait` is called, otherwise never executes
3. default policy: mix of above for the purpose of avoiding oversubscription, load balancing, thread construction/destruction.
#### B. using `std::async` you cannot predict
1. whether `f` will run concurrently with `t`
2. whether `f` runs on a thread different from the thread invoking `get` or  `wait` or `fut`
3. whether `f` runs
#### C. what it affects
1. cannot predict which thread's TLS (thread-local storage) will be accessed
2. `wait`-based loops may never finish
### 37. Make `std::threads` unjoinable on all paths - REREAD
1. make `std::thread`s unjoinable on all paths
2. `join`-on-destruction can lead to difficult-to-debug performance anomalies
3. `detach`-on-destruction can lead to difficult-to-debug undefined behavior
4. declare `std::thread` objects last in lists of data members
### 38. Be aware of varying thread handle destructor behavior
1. future destructors normally just destroy the future's data members
2. the final future referring to a shared state for a non-deferred task launched via `std::async` blocks until the task completes
#### A. one lone exception
- arises when all below applies
    - it refers to a shared state that was created due to a call to `std::async`
    - the task's launch policy is `std::launch::async`, either because that was chosen by runtime system or because it was specified
    - the future is the last future referring to the shared state
- the behavior: block until the async running task completes
### 39. Consider `void` futures for one-shot event communication
#### A. normal way of wait-notify
1. use conditional variable, issues (a. lost-wakeup; b. spurious wakeup)
2. use a flag/atomic int (semaphore actually) to address this issue
#### B. neat way - future
1. void future
2. set first join later, dont dtoraction::join, because if exception in midway, the function would hang
### 40. use `std::atomic` for concurrency, `volatile` for special memory
1. `std::atomic` is for data accessed from multiple threads without using mutexes. It's a tool for writing concurrent software.
2. `volatile` is for memory where reads and writes should not be optimized away. It's a tool for working with special memory
#### A. use
1. atomic is used against compiler/hardware reordering, no code in the source code precedes a write of `std::atomic` variable may take place afterwards.
2. volatile is used against compiler dead load/store elimination
#### B. note: implementation of `atomic` operations
1. bus lock: lock the whole bus, all CPU cores cannot access memory if one core is doing amotic operation
2. cacheline lock: invalidate all core's corresponding cacheline by lock it. Afterwards it writeback and validate it.
## Chapter 8
### 41. Consider pass by value for copyable parameters that are cheap to move and always copied
1. for copyable, cheap-to-move parameters that are always copied, pass by value may be nearly as efficient as pass by reference, it's easier to implement, and it can generate less object code
2. copying parameters via construction may be significant more expensive than copying them via assignement
3. pass by value is subject to the slicing problem, so it's typically inappropriate for base class parameter types
### 42. Consider emplacement instead of insertion
#### A. if all the followings are true, emplacement will almost certainly outperform insertion
1. the value being added is constructed into the container, not assigned
2. the argument type(s) being passed differ from the type held by the container
3. the container is unlikely to reject the new value as a duplicate
#### ToR
1. in principle, emplacement functions should sometimes more efficient than their insertion counterparts, and they should never be less efficient
2. emplacement functions may perform type conversions that would be rejected by insertion functions.