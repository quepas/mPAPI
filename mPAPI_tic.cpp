#include <mex.h>
#include <papi.h>
#include <cmath>
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
    double *events = mxGetPr(prhs[0]);
    for (size_t k = 0; k < mxGetNumberOfElements(prhs[0]); ++k)
    {
        int event_set = static_cast<int>(round(events[k]));
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
}
