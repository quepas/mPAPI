#include <mex.h>
#include <papi.h>
#include <cmath>
#include "mPAPI_utils.hpp"
#include <fstream>

/*
 *  mPAPI_trace_toc -- finish
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int event_set = mPAPI_get_int32_scalar(mxGetFieldByNumber(prhs[0], 0, 0));
    int num_events = mPAPI_get_int32_scalar(mxGetFieldByNumber(prhs[0], 0, 5));
    long long values[num_events];
    int retval;

    if ((retval = PAPI_stop(event_set, values)) != PAPI_OK)
        mPAPI_mex_error_with_reason("Failed to start the profiling.", retval);
    if ((retval = PAPI_reset(event_set)) != PAPI_OK)
        mPAPI_mex_error_with_reason("Failed to reset the counters.", retval);
}
