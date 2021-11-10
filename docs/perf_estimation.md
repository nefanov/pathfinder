###CPU

### Absolute values
The most theoretically proven and at the same time the simplest way of performance estimation is the calculation of peak float-points operations per second, which your CPU can achieve.

The main disadvantage that it is related only to float-point operations, so __scenario__ is restricted to this class of calculations. Also, hardware properties matters.
Suppose your CPU is Intel x86-64 and:
- Has 4 cores, on which you set affinity of your calculation threads 1 thread per core
- Has max turbo boost frequency 4 GHz
- supports AVX-2
- has two FMA units that can operate on 256-bit wide vectors. 

Then, one core can perform 2∗4∗2 double-precision floating-point multiply and adds per cycle, 
which at the max turbo boost frequency of 4 GHz translates to 4 * 16 = 64 GFLOPS per core. 
(Frequency scaling can impact on this calculation, but lets ignore it.). 
So if your threads not communicate between and not use shared resources, you potentially can get 4*64=256 GFLOPS per cycle.

### Relative values
More generalizable way.
First of all, you need __scenario__.

Measure the __scenario__ before and after the improvement of investigated subsystem (SW Module, HW, architecture, etc.). 
The metrics may differs, but let it be, for example, CPU cycles.
Then, new/old * 100% is percentage of performance gain.
