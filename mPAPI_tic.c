#include "mex.h"
#include "matrix.h"
#include "papi.h"

#include "mPAPI_status.h"

/*
 *  mPAPI_tic -- start hardware performance monitoring counters.
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int num_counters = mPAPI_getNumRunningCounters();
    if (num_counters > 0)
    {
        int result;
        long long counter_values[num_counters];
        if ((result = PAPI_read_counters(counter_values, num_counters)) != PAPI_OK)
        {
            char error[256];
            sprintf(error, "Failed to clear counters. "
                           "Error message: %s.",
                    PAPI_strerror(result));
        }
    }
    else
    {
        mexPrintf("Counters are unregistered and not running. Execute mPAPI_register first.\n");
    }
}
