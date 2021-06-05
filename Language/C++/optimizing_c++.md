# Optimizing C++
## Choosing the right platform
the distinctions between RISC and CISC, PC's and mainframes, and between simple processors and vector processors are becoming increasing blurred as the standard PC processors with CISC instruction sets have got RISC cores, vector processing instructions, multiple cores, and a processing speed exceeding that of yesterday's big mainframe computers
### I. Choice of compiler
1. MSVC: friendly, good debugger, not the best at optimizations
2. GCC: best optimizing compilers
3. Clang: in many cases, it's the best
## Finding the biggest time consumers
### I. Profiler
#### A. profiler methods
1. instrumentation: compiler inserts extra code at each function call to count how many time function called & how much time it takes
2. debugging: inserts break points at every function
3. time-based sampling: compiler tells OS to interrupt every 10ms
4. event-based sampling: generate interrupts at certain events, for example, every time a thousand cache misses have occured
5. this methods have problems (coarse time measurement, execution time too small/too long, interference from other processes, user input, function addresses are obsecured in optimized programs), we can put measurement instruments directly into the code rather than using a ready-made profiler. (use `sched_setaffinity` to prevent thread from jumping between loops)
#### C. time killer
1. program loading: JIT
2. Dynamic linking and position-independent code
3. file access
4. system database - register table
5. other database
6. graphics - api call can take long take, because across multiple layers, need to switch to protected mode and back again
7. other system resources
8. network access
9. memory access
10. context switches - can be reduced by having a multiple core CPU / multiple CPU
11. dependency chains - shouldnt be too long
12. execution unit throughputs - optimization can be done to this level to achieve highest efficiency
## The efficiency of different C++ constructs
### I. different kinds of variable storage
#### stack
- fast (high probability in the cache)
- free on function exit
#### global or static storage
- for global variables, and static variables
- also for floating point constants, string constants, array initializer lists, `switch` statement jump tables, and virtual function tables
- do not make variables global if you can avoid it -> slow
- for `static` variable in function, everytime function executes, need to check if it's first time calling the function to decide whether or not to initialize the variable. Using `static const` if it's constant to avoid unnecessary  check
#### register storage
- fast
- can use for multiple variables if their live ranges do not overlap
- limited
#### volatile
- wont optimize dead load/store elimination for volatile variables
- does not mean atomic
- prevents compiler from storing it to register
#### thread-local storage
- keyword `thread_local`, `__thread` or `__declspec(thread)`
- accessed through a pointer stored in a thread environment block
- static thread-local storage should be avoid, if possible, and replaced by storage on the thread's own block
#### Far
-
#### dynamic memory allocation
- new/delete, malloc/free
- consume a significant amount of time (system call -> sbrk -> allocate memory)
- heap can easily become fragmented
- garbage collection can take a lot of time
#### variables declared inside a class
- stored in the order in which thry appear in the class declaration
- storing variables in a class or structure is a good way of making sure that variables that are used in the same part of the program are also stored near each other
### II. Integer variables and operators
#### integer sizes
- the compiler will always select the most efficient size if you declare an `int`, integers of smaller sizes are only slightly less efficient in many cases
- recommanded to use default integer size in the cases where size does not matter and there is no risk of overflow, such as (a) simple variables, (b) loop counters
- in large arrays, it may be preferred to use the smallest integer size for less cache miss
- unsigned integer type `size_t` is 32 bits in 32-bit machine, 64 bits in 64-bit machine
#### signed versus unsigned integers
- a few cases that signed is different from unsigned
    - division by a constant is faster with unsigned
    - conversion to floating point is faster with signed
    - overflow of an unsigned variable produces a low positive result; while overflow of a signed will cause undefined behavior
- conversion between signed and unsigned is costless (done only in compile-time)
- not to mix signed and unsigned integers in comparisons, it's ambiguous and may produce undesired results
#### integer operations
- add, sub - 1 clock
- mul - 4 clock
- div - 40 ~ 80 clocks
#### increment and decrement operators
- same if result not used
- `array[++i]` will be faster than `array[i++]` because it can calculate address earlier
- `a = ++b` is faster than another because compiler know they are identical and thus can assign same register for both
### III. Floating point variables and operations
- modern compilers will use vector registers for floating point calculations
- double precision usually take no more time than single precision
    - single precision division, sqrt, and math functions are faster than double
    - add/sub/mul is the same for single and double
- use double without worring too much about the costs
- use single if you have big arrays and want to utilize cache
- do not mix single and double in the same expression
- avoid conversions between integers and floating point variables (will copy)
- set the flush-to-zero mode unless you have special reasons to use subnormal numbers. You may, in addition, set the denormals-are-zero-mode if vector registers are available
#### efficiency
- add, sub - 3 ~ 6 clocks
- mul - 4 ~ 8 clocks
- div - 14 ~ 45 clocks
### IV. Enums
- simply an integer in disguise, as efficient as integers
### V. Booleans
#### A. the order of boolean operands
- for `&&`, put fail first and success last
- for `||`, put success first and fail last
- put the most predictable operand first
- put the operand that is calculated the fastest first
#### B. boolean variables are overdetermined
- can use `char` to replace `bool` and `&` `|` over `&&` `||` if:
    - both are variables, not comparisons, latter is not expression should not be evaluated depends on former results
#### C. boolean vector operations
- an integer may be used as a Boolean vector, only 1 cycle
### VI. Pointers and references
#### A. pointers vs references
- equally efficient because they are in fact doing the same thing
- advantages of pointers over references
    - pointers are more clear, you dont know reference is reference
    - pointer can do arithmetic operations
- disadvantages of pointers over references
    - syntax simpler
    - references are safer because they are sure to point to a valid address
    - references are useful for copy constructors and overloaded operators
    - function parameters that are declared as constant references accept expressions as arguments while pointers and non-constant references require a variable
#### B. efficiency
- fast because dedicated optimizations
- disadvantages
    - requires an extra register to hold the value of value of the pointer / reference
    - value of the pointer is needed a few clock cycles before the time the variable pointed to can be accessed
#### C. pointer arithmetic
- the object pointed to can be accessed approx 2 clock cycles after the value of the pointer has been calculated
### VII. function pointers
- calling a function through a function pointer may take a few clock cycles more than calling the function directly if the traget address can be predicted
- target address is predicted if the value of the function pointer is the same as last time the statement was executed
### VIII. member pointers
- data member stores the offset of a data member relative to the beginning of the object
- member function is simply the address of the member function
- a compiler has to use the most complicated implementation of the member pointers if it has incomplete information about the class that the member pointer refers
- use compiler option to generate simplist class layout
### IX. smart pointers
- no extra cost to accessing an object through a smart pointer, but there is an extra cost whenever a smart pointer is created, deleted, copied, or transferred from one function to another
- smart pointers can be useful in the situation where the logical structure of a program dictates that an object must be dynamically created by one function and later deleted by another function and these two functions are unrelated to each other
- if a program uses many small dynamically allocated objects with each their smart pointer then you may consider if the cost of this solution is too high
### X. arrays
- size of all but the first dimension may preferably be a power of 2 if rows are indexed in a non-sequential order in order to make the address calculation more efficient
### XI. type conversions
#### A. signed/unsigned conversion
- no checking for overflow
- takes no time
#### B. integer size conversion
- one clock cycle if the source is an arithmetic expression
- no extra time for the rest, no overflow checking
#### C. floating point precision conversion
- no cost if floating point register stack is used
- it takes between 2 and 15 clock cycles when the XMM registers are used
#### D. integer to float conversion
- conversion of a signed integer to a `float` or `double` takes 4 ~ 16 clock cycles
- conversion of an unsigned integer takes longer time unless the AVX512 instruction set is enabled
#### E. float to integer conversion
- takes a very long time unless the SSE2 or later instruction set is enabled
- typically, the conversion takes 50 ~ 100 clock cycles
##### tips
- avoid the conversions by using different types of variables
- move the conversion out of the innermost loop by storing intermediate results as floating point
- use 64-bit mode or enable the SSE2 instruction set
- use rounding instead of truncation and make a round function using assembly language (?)
#### F. pointer type conversion
- no cost
#### G. re-interpreting the type of an object
```
float x;
*(int *)&x |= 0x80000000; // set sign bit of x
```
- disadvantage: force compiler to store x in memory rather than in a register (why?)
##### dangers when type-casting pointers
- the trick voilates the strict aliasing rule of standard C, specifying that two pointers of different types cannot point to the same object
- trick will fail if the object is treated as bigger than it actually is (when `size of int` != `size of float`)
- if you access part of a variable, for example 32 bits of 64 bits varaible, the code will not be portable to platforms that use big endian storage
- if you access a variable in parts, the code is likely to execute slower than intended because of a store forwarding delay in the CPU
#### H. const cast
- no cost
#### I. static cast
- no cost, C style
#### J. reinterpret cast
- no cost, C style casting with a little more syntax check
#### K. dynamic cast
- used to convert pointer to class
- runtime check validity of conversion, slower but safer
#### L. converting class objects
- are possible only if the programmer has defined a constructor, and overloaded assignment operator, or an overloaded type casting operator that specifies how to do the conversion
### XII. Branches and switch statements
#### A. branch
- 0 ~ 2 if predicted
- 12 ~ 25 if mispredicted
#### B. switch
- small sequence: jump table
- large far sequence: binary search on sorted value-func pair (book say branch tree, not right)
#### C. target branch buffer
- for branch and function calls
- contention occurs if a program has many branches or function calls
- contentions reduce predicted rate
#### D. optimization
- can replace a poorly predictable branch by a table lookup
### XIII. Loops
- a loop with a small and fixed repeat count and no branches inside can be predicted perfectly
#### A. loop unrolling
- advantage:
    - eliminate `if` branch
- disadvantage:
    - take up more space in the `code cache`
    - dont do unrolling when `lookback buffer` is utilizable, or processors having a micro-op cache
    - repeat count cannot be divided
#### B. loop control condition
- compare to 0 is more efficient, when `i` is not used as index, write `i = N - 1; i >= 0; i--` instead of `i = 0; i < N; i++`; when `i` is used as array index, still forward, cuz data cache is optimzied for array forwards
- loop counter should preferably be an integer, integer comparison is cheaper than float comparison
- faster to use `memset` and `memcpy`, no index addressing, no index increment/decrement
### XIV. functions
- may slow down a program for the following reasons
    - makes the microprocessor jump to a different code address and back, ~4 clock cycles
    - code cache works less efficiently
    - params may be passed by stack
    - extra time is needed for setting up a stack frame, saving and restoring registers, and possibly save exception handling information
    - function call occupies space in BTB
#### A. avoid unnecessary functions
- focus on logic not length of function
#### B. use inline functions
#### C. avoid nested function calls in the innermost loop
#### D. use macros instead of functions
- it's inline
- note
    - the same expression will be evaluate for every use in macro
    - name should be long and unique
#### E. use fastcall and vectorcall functions
- `__fastcall` use under 32-bit mode to pass first 2 params by register
- `__vectorcall` changes the function calling method in Windows system for floating point and vector operands so that parameters are passed and returned in vector registers
#### F. make functions local
- make it easier for compiler to inline the function and to optimize across function calls
- ways
    - add keyword `static` to the function declaration, but cannot use for class member function (different meaning)
    - put the function or class into an anonymous namespace
    - gnu compilers allows `__attribute__((visibility("hidden")))`
#### G. use whole program optimization
- use compiler option to make only one final object file, so compiler can optimize better
- cannot be used for function libraries distributed as object or library files
#### H. use 64-bit mode
- windows: passed by 4 registers regardless of integer/floating point
- linux: can pass by 6 integer register + 8 floating point register
- on 32-bit mode, same
### XV. function parameters
- params can be passed via register if all following conditions are met, if not met, better pass by pointer of object.
    - object is small enough to fit into a single register
    - has no copy constructor and no desctructor. otherwise need to copy when call, destruct when return
    - no virtual member
    - does not use run type type identification (RTTI)
- the preferred methods for transferring composite objects to a function
    - pass by a const reference, it accepts rvalue!
    - make the function a member of the object's class or structure
### XVI. function return types
- return type should preferably be a simple type, a pointer, a reference, or void
- objects of a composite type can be returned in registers only in the simplest cases: calling conventions for different C++ compilers and operating systems
- except for the simplest cases, composite objects are returned by copying them into a place indicated by the caller through a hidden pointer. The copy constructor, if any, is usually called in the copy process, and the destructor is called when the original is destroyed.
- instead of returning a composite object, you may consider the following alternatives
    - make the function a constructor for the object
    - make the function modify an existing object rather than making a new one
    - make the function return a pointer or reference to a static object defined inside the function. This is efficient, but risky.
    - make the function construct an object with `new` and return a pointer to it. Inefficient because of the costs of dynamic memory allocation.
### XVII. function tail calls
- if the last statement of a function is a call to another function, the compiler can replace the call by a jump to the second funcition (use same stack space!!!)
### XVIII. recursive functions
- deep recursion makes the prediction of return addresses less efficient (see `return stack buffer` in manual 3) #TODO
- recursive tail calls are more efficient than other recursive calls, but still less efficient than a loop
### XIX. structures and classes
- advantages
    - variables that are used together are also stored together
    - variables that are members of a class need not be passed as params
- disadvantages
    - inefficient to dividing code into too many small classes
    - overhead of transfer `this` pointer (time, it takes up one register)
    - virtual members are less efficient
### XX. class data members
- data members stored consecutively in the order in which they are declared (in struct yes, in class is it? `inside c++ object model` said not #TODO)
- align in a way that the data members are most compact
- put big arrays and other big objects last in a structure/class
### XXI. class member functions (methods)
- calling a member function is as fast as calling a simple function
- static function doesnt need `this` pointer, thus it's faster
### XXII. virtual member functions
- the time it takes to call a virtual member function is a few clock cycles more than it take to call a non-virtual member function, provided the function call statement always calls the same version of the virtual function.
- the ruls for prediction and misprediction of virtual function is the same as for switch statements
- runtime polymorphism is needed only if it cannot be known at compile time which version of a polymorphic member function is called
- if used in critical part: a. use without polymorphism; b. compile-time polymorphism
- use template instead if polymorphism whenever possible
### XXIII. runtime type identification (RTTI)
- adds extra information to all class objects and it's not efficient, if compiler has an option for RTTI then turn it off
### XXIV. inheritance
- there may be a degradation in code caching and data caching for the following reasons
    - child class includes all data members of the parant class
    - size of the parant class data member is added to the offset of the child class members
    - member functions of parant and child are possibly stored in different modules
- inheritance from multiple parant classes in the same generation can cause complications with member pointers and virtual functions of when accessing an object of a derived class through a pointer to one of the base classes.
### XXV. constructors and destructors
- as efficient as others
### XXVI. unions
- data members share the same memory space
### XXVII. bitfields
- it's faster to compose a bitfield by the use of `<<` and `|` operations
```
Union Bitfield {
    struct {
        int a:4;
        int b:2;
        int c:2;
    }
    char abc;
}
Bitfield x;
int A, B, C;
x.abc = A | (B << 4) | (C << 6);
```
### XXVIII. overloaded functions
- simply treated as different functions
### XXIX. overloaded operators
- overloaded operator is equivalent to a function
- an expression with multiple overloaded operators will cause the creation of temporary objects for intermediate results, which may be undesired. Compiler can optimize simple cases
### XXX. templates
#### A. CRTP (curiously recurring template pattern)
- grandparent layer - all non-polymorphic functions (actually can put into parent layer as well)
- parent layer - polymorphic layer, take child class as a template parameter
- child layer - implementation of polymorphic functions
### XXXI. threads
- overheads of multithreading
    - start and stop threads: do not put task into separate thread if it is short in duration compared with the time it takes to start and stop the thread
    - task switching
    - synchronize and communicate between threads: cost of semaphore, mutexes is considerable. variable shared between multiple threads also need to be declared `volatile` to prevent load in register
    - different threads need separate storage
### XXXII. exceptions and error handling
- even if no error occurs, the program may have to do a lot of bookkeeping in order to know how to recover in tht event of an exception. the costs largely dependes on the compiler. some compilers have efficient table-based method with little or no overhead while other have inefficient code-based methods or require runtime type identification (RTTI)
- can use `noexcept` to disable it
#### A. exceptions and vector code
- vector instructions are useful for doing multiple calculations in parallel
- exception handling does not work well with vector code because a single element in a vector may cause an exception while perhaps the other vector elements do not
- if the code can benefit from vector instructions, then it is better to disable the trapping of exceptions and rely on the propagation of NAN and INF instead.
#### B. avoiding the cost of exception handling
#### C. making exception-safe code
### XXXIII. stack unwinding
- a mechanism used by exception handlers for cleaning up and calling any necessary destructors after jumping out of a functino in a case of an exception without using the normal return route
- also used in two other situations
    - can be used then thread terminated
    - is also used when the function `longjmp` is used for jumping out of a function. Avoid the use of `longjmp` is possible
### XXXIV. Propagation of `NAN` and `INF`
- let it propagate, efficient
### XXXV. preprocessing directives
- `#if` can be used for multiple platform/config
### XXXVI. namespaces
- no cost
