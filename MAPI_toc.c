#include "mex.h"
#include "matrix.h"
#include "papi.h"

#include "MAPI_status.h"

/*
 *  MAPI_toc -- read values from hardware performance monitoring counters.
 */
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
    int num_counters = MAPI_getNumRunningCounters();
    if (num_counters > 0) {
        plhs[0] = mxCreateNumericMatrix(1, num_counters, mxINT64_CLASS, mxREAL);
        long long *counter_values = mxGetData(plhs[0]);
        int result;
        if ((result = PAPI_read_counters(counter_values, num_counters)) != PAPI_OK) {
            char error[256];
            sprintf(error, "Failed to read counters. "
                           "Error message: %s.", PAPI_strerror(result));
        }
    } else {
        mexPrintf("Counters are unregistered and not running. Execute MAPI_register first.\n");
    }
}
