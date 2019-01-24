#include <mex.h>
#include <papi.h>
#include "mPAPI_utils.hpp"

/*
 *  mPAPI_tic -- start hardware performance monitoring counters.
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (!mexIsLocked())
    {
        mexLock();
    }
    int event_set = mPAPI_get_event_set();
    if (event_set == PAPI_NULL)
    {
        mexErrMsgTxt("Register performance events before reading them.");
    }
    int retval;
    if ((retval = PAPI_start(event_set)) != PAPI_OK)
    {
        mPAPI_mex_error_with_reason("Failed to start performance counters", retval);
    }
}
