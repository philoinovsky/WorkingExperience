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
#### D. integer to float conversion
#### E. float to integer conversion
