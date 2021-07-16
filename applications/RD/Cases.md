1.

2. Cyclic function call (intraprocedural).
Avoid the cyclic calls to function, which result is unchanged during the cycle.
Intraprocedural mode means that the context of callee will not be analysed during the pattern matching.

It takes a list of restriction on function and thus on the pattern:
* Function arguments should be constants, string literals or absent. 
* Function should not change any global variables.
* Function should not use any global varialbles which can be changed during the cycle.
* Return value of function should be used, else function call is redundant.

To meet such restrictions, current pattern only supports:
strlen
strcmp

```C
int i=0;
```

3. Cyclic function call (interprocedural)
Avoid the cyclic calls to function, which result is unchanged during the cycle.
Interprocedural mode means that the context of callee will be analysed during the pattern matching.

4. Item 3
