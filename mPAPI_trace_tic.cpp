#include <mex.h>
#include <papi.h>
#include <cmath>
#include "mPAPI_utils.hpp"
#include <fstream>

/*
 *  mPAPI_trace_tic -- start a new sub-trace
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int event_set = mPAPI_get_int32_scalar(mxGetFieldByNumber(prhs[0], 0, 0));
    std::string perf_events = mxArrayToString(mxGetFieldByNumber(prhs[0], 0, 3));
    std::string trace_file_name = mxArrayToString(mxGetFieldByNumber(prhs[0], 0, 6));
    std::string header = mxArrayToString(prhs[1]);

    std::ofstream trace_file(trace_file_name, std::fstream::app);
    trace_file << "@trace:" << header << std::endl;
    //trace_file << perf_events << std::endl;
    trace_file.close();

    int retval;
    if ((retval = PAPI_start(event_set)) != PAPI_OK)
        mPAPI_mex_error_with_reason("Failed to start the profiling.", retval);
}
