# mPAPI

Simple [**MATLAB**](https://www.mathworks.com/products/matlab.html) API for [**PAPI**](http://icl.cs.utk.edu/papi/) (Performance Application Programming Interface).

## Properties

* Hardware counters are measured for the parent and child threads (e.g. when using parallelized functions like `sum`). Unfortunately, there is no way to differentiate which counters come from which thread.
* Each function in the MEX-file is locked (once loaded it can't be erased using `clear` function in MATLAB environment)

## Installation

1. Install [PAPI >=5.5.1](http://icl.cs.utk.edu/papi/)
2. Build mPAPI functions: `mPAPI_register`, `mPAPI_tic`, `mPAPI_toc`, `mPAPI_groupEvents`, `mPAPI_enumNativeEvents`, `mPAPI_enumPresetEvents` with MEX-compatible compiler (the repository contains two bash script for building `build.sh` and `build_all.sh`):
```
mex -I/usr/local/include mPAPI_register.c -L/usr/local/lib/ -lpapi -output mPAPI_register
mex -I/usr/local/include mPAPI_tic.c -L/usr/local/lib/ -lpapi -output mPAPI_tic
mex -I/usr/local/include mPAPI_toc.c -L/usr/local/lib/ -lpapi -output mPAPI_toc
mex -I/usr/local/include mPAPI_groupEvents.c -L/usr/local/lib/ -lpapi -output mPAPI_groupEvents
mex -I/usr/local/include mPAPI_enumNativeEvents.c -L/usr/local/lib/ -lpapi -output mPAPI_enumNativeEvents
mex -I/usr/local/include mPAPI_enumPresetEvents.c -L/usr/local/lib/ -lpapi -output mPAPI_enumPresetEvents
```

Where directory ``/usr/local/include`` contains ``papi.h`` header and directory ``/usr/local/lib/`` contains ``libpapi.so`` static library.

## Usage

1. Register hardware performance monitoring counters (PMC) using _preset_ or _native_ events:
  * For the current thread/process:
      ```matlab
      ev = mPAPI_register({'FP_ARITH:SCALAR_SINGLE', 'L1D:REPLACEMENT', 'PAPI_L2_ICA'})
      ```
  * In multiplex mode for the current thread:
      ```matlab
      ev = mPAPI_register({'FP_ARITH:SCALAR_SINGLE', 'L1D:REPLACEMENT', 'PAPI_L2_ICA'}, true)
      ```
  * For a specific thread/process by PID:
      ```matlab
      ev = mPAPI_register({'PAPI_TOT_INS'}, 1234)
      ```
3. Start counters for the specific event-set(s):
```matlab
mPAPI_tic(ev)
```
4. Read counters measurements 
  * For the specific event-set:
      ```matlab
      >> mPAPI_toc(ev)
      ans = [0, 1559, 4032]
      ```
  * For many event-sets:
      ```matlab
      >> mPAPI_toc([ev1, ev2])
      ans = [0, 1559, 4032;
             0, 1450, 3999]
      ````
5. Enumarate all available native or preset PAPI events:
```matlab
>> mPAPI_enumNativeEvents()
ans = {'ix86arch::UNHALTED_CORE_CYCLES', 'ix86arch::INSTRUCTION_RETIRED', ...}
>> mPAPI_enumPresetEvents()
ans = {'PAPI_L1_DCM', 'PAPI_L1_ICM', ...}
```
6. Divide events into compatible groups (that can be measured simultaneously)
```matlab
>> mPAPI_groupEvents({'PAPI_L1_DCM', 'PAPI_L1_ICM', ...})
ans = {{'PAPI_L1_DCM', 'PAPI_L1_ICM', ...},
       ...
      }
```
## Problems

In order to set an older version of _GCC_ (newer might not be supported by MATLAB's MEX compiler), run _mex_ as follows:

```matlab
mex GXX='/usr/bin/gcc-X.X' ... % R2013a/R2015b/R2018b
```

## Comments

* The number of hardware counters available on the system defines the upper limit of counters you can register using ``mPAPI_register`` function.
* Not all hardware counters can be mixed and used simultaneously (except when in multiplex mode).
