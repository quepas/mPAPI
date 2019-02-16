#include <mex.h>
#include <matrix.h>
#include <papi.h>
#include <cmath>
#include <vector>
#include "mPAPI_utils.hpp"

/*
 *  mPAPI_toc -- read values from hardware performance monitoring counters.
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (!mexIsLocked())
    {
        mexLock();
    }

    size_t K = mxGetNumberOfElements(prhs[0]);

    // Check num. of first event set
    double *events = mxGetPr(prhs[0]);
    int event_set = static_cast<int>(round(events[0]));
    int event_set_size = PAPI_num_events(event_set);

    plhs[0] = mxCreateNumericMatrix(K, PAPI_num_events(event_set), mxINT64_CLASS, mxREAL);
    long long *output = (long long *)mxGetData(plhs[0]);

    std::vector<std::vector<long long>> results;

    for (size_t k = 0; k < K; ++k)
    {
        event_set = static_cast<int>(round(events[k]));
        if (event_set == PAPI_NULL)
        {
            mexErrMsgTxt("Register performance events before reading them.");
        }
        event_set_size = PAPI_num_events(event_set);
        long long counter_values[event_set_size];

        int retval;
        if ((retval = PAPI_stop(event_set, counter_values)) != PAPI_OK)
        {
            mPAPI_mex_error_with_reason("Failed to stop and read PAPI counters", retval);
        }

        // Insert into a vector
        std::vector<long long> line;
        for (int i = 0; i < event_set_size; ++i)
        {
            line.push_back(counter_values[i]);
        }
        results.push_back(line);
    }

    // TODO: pack results into a cell array!
    for (unsigned int row = 0; row < K; row++)
    {
        for (unsigned int col = 0; col < event_set_size; ++col)
        {
            output[row + col * event_set_size] = results[row][col];
        }
    }
}
