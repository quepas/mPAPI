# mPAPI

Simple [**MATLAB**](https://www.mathworks.com/products/matlab.html) API for [**PAPI**](http://icl.cs.utk.edu/papi/) (Performance Application Programming Interface).

## Installation

1. Install [PAPI >=5.5.1](http://icl.cs.utk.edu/papi/)
2. Build mPAPI functions: mPAPI_register, mPAPI_tic, mPAPI_toc with MEX-compatible compiler:
```
mex -I/usr/local/include mPAPI_register.c -L/usr/local/lib/ -lpapi -output mPAPI_register
mex -I/usr/local/include mPAPI_tic.c -L/usr/local/lib/ -lpapi -output mPAPI_tic
mex -I/usr/local/include mPAPI_toc.c -L/usr/local/lib/ -lpapi -output mPAPI_toc
```

Where directory ``/usr/local/include`` contains ``papi.h`` header and directory ``/usr/local/lib/`` contains ``libpapi.so`` static library.

## Usage

1. Register hardware performance monitoring counters (PMC) using _preset_ or _native_ events:
```matlab
>> mPAPI_register('FP_ARITH:SCALAR_SINGLE', 'L1D:REPLACEMENT', 'PAPI_L2_ICA')
```
2. Start counters:
```matlab
>> mPAPI_tic
```
3. Read counters measurements:
```matlab
>> mPAPI_toc

ans = [0   1559   4032]
```
## Problems

In order to set older version of _GCC_ (newer might not be supported), run _mex_ as follow:

```matlab
mex GCC='/usr/bin/gcc-4.9' ...
```

## Comments

* The number of hardware counters available on the system defines the upper limit of counters you can register using ``mPAPI_register`` function.
* Not all hardware counters can be mixed and used simultaneously.
