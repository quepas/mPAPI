#include <mex.h>
#include <papi.h>

#include "mPAPI_status.h"

/*
 *  mPAPI_tic -- start hardware performance monitoring counters.
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int event_set = mPAPI_get_event_set();
    if (event_set == PAPI_NULL)
    {
        mexErrMsgTxt("Register performance events before reading them.");
    }
    int result;
    if ((result = PAPI_start(event_set)) != PAPI_OK)
    {
        char error[PAPI_HUGE_STR_LEN];
        sprintf(error,
                "Failed to start performance counters. Reason: %s.",
                PAPI_strerror(result));
        mexErrMsgTxt(error);
    }
}
