# MAPI
MAPI is an extended MATLAB API for PAPI (5.5.1 and beyond). Just PAPI MAPI API for PAPI.

## Installation

1. Install [PAPI >=5.5.1](http://icl.cs.utk.edu/papi/)
2. Build MAPI functions: MAPI_register, MAPI_tic, MAPI_toc with MEX-compatible compiler:
```
mex -I/usr/local/include MAPI_register.c -L/usr/local/lib/ -lpapi -output MAPI_register
mex -I/usr/local/include MAPI_tic.c -L/usr/local/lib/ -lpapi -output MAPI_tic
mex -I/usr/local/include MAPI_toc.c -L/usr/local/lib/ -lpapi -output MAPI_toc
``` 
## Usage

1. Register hardware performance monitoring counters (using _preset_ and _native_ events):
```
>> MAPI_register('FP_ARITH:SCALAR_SINGLE', 'L1D:REPLACEMENT', 'PAPI_L2_ICA')
```
2. Clear counters:
```
>> MAPI_tic
```
3. Get counters measurements:
```matlab
>> MAPI_toc

ans = [15   662645   648020]
```

## Comments

* The number of hardware counters available on the system defines the upper limit of counters you can register using ``MAPI_register`` function.

