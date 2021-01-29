# Python
## Coding Style
1. use `tab` instead of `spaces`
2. maximum line length -> 160 chars
3. don't use `eval`
4. use `None` instead of using mutable variables as default parameter values, (i.e `[]`, `{}`)
5. use `log` instead of `print`
6. use `import xxx` or `from xxx import yyy` instead of `from xxx import *`, `import *` is only allowed for basic modules
7. call `dict.get(key)` instead of `dict[key]`
8. use `list comprehensions` instead of `filter` or `map` for better performance and readability
9. if `if` contains a return statement, the `else` becomes unnecessary. Handle spacial case using `if` block and return, handle normal case outside the block.
## Library
1. use `requests` instead of `urllib`/`urllib2`/`httplib`/`httplib2`
## Performance Tips
1. sort can take an optional comparison function as an argument. This is slow. can use DSU (Decorate Sort Undecorate).
```
def sortby(somelist, n):
    nlist = [(x[n], x) for x in somelist]
    nlist.sort()
    return [val for (key, val) in nlist]
```
```
def sortby_inplace(somelist, n):
    somelist[:] = [(x[n], x) for x in somelist]
    somelist.sort()
    somelist[:] = [val for (key, val) in somelist]
    return
```
2. strings are immutable. avoid this
```
s = ""
for substring in list:
    s += substring
```
use `s = "".join(list)` instead.\
when generating bits of a string sequentially. Use a list to keep the result then join.\
avoid `out = "<html>" + head + prologue + query + tail + "</html>"`\
use `out = "<html>%s%s%s%s</html>" % (head, prologue, query, tail)`\
for better readability `out = "<html>%(head)s%(prologue)s%(query)s%(tail)s</html>" % locals()`
3. use `map` instead of `for loop`, note the `list comprehensions` are often as fast or faster than equivalant use of `map`
4. eliminate dots: use variable to store function, no need to evaluate every time
5. use local variable
6. lazy import: don't import anything globally, only import once in local function when it's needed
7. data aggregation: avoid call functions too frequently
8. `x + x` is faster than `x * 2` and `x << 1`
9. `xrange` is yielder, `range` generates and allocate list of numbers to memory



