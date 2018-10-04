#include <mex.h>
#include <matrix.h>
#include <papi.h>

#include "mPAPI_status.h"

/*
 *  mPAPI_toc -- read values from hardware performance monitoring counters.
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int event_set = mPAPI_get_event_set();
    if (event_set == PAPI_NULL)
    {
        mexErrMsgTxt("Register performance events before reading them.");
    }
    int event_set_size = PAPI_num_events(event_set);
    plhs[0] = mxCreateNumericMatrix(1, event_set_size, mxINT64_CLASS, mxREAL);
    long long *counter_values = (long long *)mxGetData(plhs[0]);

    int result;
    if ((result = PAPI_stop(event_set, counter_values)) != PAPI_OK)
    {
        char error[PAPI_HUGE_STR_LEN];
        sprintf(error,
                "Failed to stop and read PAPI counters. Reason: %s",
                PAPI_strerror(result));
        mexErrMsgTxt(error);
    }
}
