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
## Performance and usability
