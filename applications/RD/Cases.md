### 1. Cyclic function call (intraprocedural)

Avoid cyclic calls to function, which result is unchanged during the cycle.
Intraprocedural mode means that the context of callee will not be analysed during the pattern matching.

It takes a list of restriction on function and thus on the pattern:
* Function arguments should be constants, string literals or absent. 
* Function should not change any global variables.
* Function should not use any global varialbles which can be changed during the cycle.
* Return value of function should be used, else function call is redundant.

To meet such restrictions, current pattern only supports:
*strlen
*strcmp

#### Example. 

Original code:
```C
size_t i;
for (i=0; i<strlen(buf); i++) {
  //..anything..//
}
 
```
Improved code:
```C
size_t i;
size_t sz = strlen(buf);
for (i=0; i < sz; i++) {
  //..anything..//
}
```
### 2. Cyclic function call (interprocedural)

Avoid the cyclic calls to function, which result is unchanged during the cycle.
Interprocedural mode means that the context of callee will be analysed during the pattern matching.

### 3. Memory operations redundancy detection [done, be improved]

Detect calls of functions memset, malloc, etc, which are redundant in current context.

 #### Example 1.
 Original code:
```C
int* buf=(int*)malloc(sizeof(int)*SZ);
memset(buf, 0, sizeof(int)*SZ);
  //..anything..//
```
Improved code:
```C
int* buf=(int*)calloc(SZ, sizeof(int));
  //..anything..//
}
```
#### Example 2.
Original code:
```C
int i=0;
memset(buf, 0, sizeof(int)*SZ);

for (i = 0; i < SZ; i++) {
  mem[i] = i;
}
```
Improved code:
```C
int i=0;

for (i = 0; i < SZ; i++) {
  mem[i] = i;
}
```

### 4. Condition reordering detector

For complex condition check expressions it is preferrable to reorder the condition from simplest to calculationally hardest. If statement is trivially true/false because of more simple part of conditions, the hard part will not be calculated.

#### Example. 

Original code:
```C
if (complex_check() && value) 
```
Improved code:
```C
if (value && complex_check()) // if not value, then no complex_check()
```

### 5. Matrix operations reordering

Reorder of matrix operations in the goal of cache misses level decreasing.

### Moreover anti-patterns:

### 6. CWE-416: Use After Free

https://cwe.mitre.org/data/definitions/416.html

### 7. CWE-476: NULL Pointer Dereference

https://cwe.mitre.org/data/definitions/476.html

### 8. Большинство примеров из MISRA C должны реализовываться довольно просто

### 9. https://cwe.mitre.org/data/definitions/754.html
