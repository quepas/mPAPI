#include "mex.h"
#include "matrix.h"
#include "papi.h"

#include "mPAPI_status.h"

/*
 *  mPAPI_register -- registers and starts hardware performance monitoring counters.
 *  Parameters:
 *      * performance_events : cell array -- stores the names of preset or native events to measure
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (!PAPI_is_initialized())
        PAPI_library_init(PAPI_VER_CURRENT);

    int retval;
    int papi_num_counters = PAPI_num_counters();
    size_t num_events = mxGetNumberOfElements(prhs[0]);

    if (num_events < 1 || num_events > papi_num_counters)
    {
        char error[PAPI_2MAX_STR_LEN];
        sprintf(error, "mPAPI_register takes cell array with 1 to %d "
                       "counter descriptors (up to number of available "
                       "hardware performance monitoring counters)",
                papi_num_counters);
        mexErrMsgTxt(error);
    }

    // Function returns a vector of PAPI event ids
    std::vector<int> event_codes;
    // Convert event names to codes
    for (int i = 0; i < num_events; ++i)
    {
        mxArray *event_name_ptr = mxGetCell(prhs[0], i);
        if (mxIsChar(event_name_ptr))
        {
            char *event_name = mxArrayToString(event_name_ptr);
            int event_code;
            if ((retval = PAPI_event_name_to_code(event_name, &event_code)) != PAPI_OK)
            {
                mPAPI_mex_warn_with_reason("Failed to convert event name to code", retval);
            }
            else
            {
                event_codes.push_back(event_code);
            }
        }
    }

    // Create an event set
    int event_set = PAPI_NULL;
    if ((retval = PAPI_create_eventset(&event_set)) != PAPI_OK)
    {
        mPAPI_mex_error_with_reason("Failed to create an event set.", retval);
    }
    // Convert vector to array
    int *counters = (int *)mxCalloc(event_codes.size(), sizeof(int));
    for (int i = 0; i < event_codes.size(); ++i)
    {
        counters[i] = event_codes[i];
    }
    if ((retval = PAPI_add_events(event_set, counters, num_events)) != PAPI_OK)
    {
        mPAPI_mex_error_with_reason("Failed to add performance events to the event set.", retval);
    }
    // Hold id of the event set
    mPAPI_set_event_set(event_set);

    mxArray *events = mxCreateNumericMatrix(1, num_events, mxINT32_CLASS, mxREAL);
    mxSetData(events, counters);
    plhs[0] = mxDuplicateArray(events);
    mxDestroyArray(events);
}
