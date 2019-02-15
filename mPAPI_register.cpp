#include <mex.h>
#include <matrix.h>
#include <papi.h>
#include <cstring>
#include <cmath>
#include "mPAPI_utils.hpp"

/*
 *  mPAPI_register -- registers and starts hardware performance monitoring counters.
 *  Parameters:
 *      * performance_events : cell array -- stores the names of preset or native events to measure
 *      * multiplexFlag? : logical -- turn on/off the multiplexing
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (!PAPI_is_initialized())
        PAPI_library_init(PAPI_VER_CURRENT);

    if (!mexIsLocked())
    {
        mexLock();
    }

    int retval;
    int papi_num_counters = PAPI_num_counters();
    size_t num_events = mxGetNumberOfElements(prhs[0]);

    // Check multiplexing flag
    bool multiplexFlag = false;
    if (nrhs == 2 && mxIsLogicalScalarTrue(prhs[1]))
    {
        multiplexFlag = true;
        if ((retval = PAPI_multiplex_init()) != PAPI_OK)
        {
            mPAPI_mex_error_with_reason("Initialization of multiplex failed", retval);
        }
    }

    if (!multiplexFlag && (num_events < 1 || num_events > papi_num_counters))
    {
        char error[PAPI_2MAX_STR_LEN];
        sprintf(error, "mPAPI_register takes cell array with 1 to %d "
                       "counter descriptors (up to number of available "
                       "hardware performance monitoring counters)",
                papi_num_counters);
        mexErrMsgTxt(error);
    }

    std::vector<int> event_codes;
    mPAPI_event_names_cell_to_codes(prhs[0], num_events, event_codes);

    // Create an event set
    int event_set = PAPI_NULL;
    if ((retval = PAPI_create_eventset(&event_set)) != PAPI_OK)
    {
        mPAPI_mex_error_with_reason("Failed to create an event set.", retval);
    }
    if (multiplexFlag)
    {
        if ((retval = PAPI_assign_eventset_component(event_set, 0)) != PAPI_OK)
        {
            mPAPI_mex_error_with_reason("Failed to assign a component to the event set.", retval);
        }
        if ((retval = PAPI_set_multiplex(event_set)) != PAPI_OK)
        {
            mPAPI_mex_error_with_reason("Failed to setup the event set as multiplexed.", retval);
        }
    }
    // Count for children as well (important for multi-thread execution)
    if (retval = PAPI_assign_eventset_component(event_set, 0) != PAPI_OK)
    {
        mPAPI_mex_error_with_reason("Failed to assign an event set to the 0-component.", retval);
    }
    if (nrhs == 2 && mxIsDouble(prhs[1]))
    {
        unsigned long pid = static_cast<unsigned long>(round(mxGetScalar(prhs[1])));
        if (retval = PAPI_attach(event_set, pid) != PAPI_OK)
        {
            mPAPI_mex_error_with_reason("Failed to attach an event set to the process.", retval);
        }
    }

    PAPI_option_t opt;
    std::memset(&opt, 0x0, sizeof(PAPI_option_t));
    opt.inherit.inherit = PAPI_INHERIT_ALL;
    opt.inherit.eventset = event_set;
    if ((retval = PAPI_set_opt(PAPI_INHERIT, &opt)) != PAPI_OK)
    {
        mPAPI_mex_error_with_reason("Failed to inherit events counting to child processes.", retval);
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
    //mPAPI_set_event_set(event_set);

    /*mxArray *events = mxCreateNumericMatrix(1, num_events, mxINT32_CLASS, mxREAL);
    mxSetData(events, counters);
    plhs[0] = mxDuplicateArray(events);
    mxDestroyArray(events);*/
    mxArray *return_val = mxCreateDoubleScalar(event_set);
    plhs[0] = mxDuplicateArray(return_val);
    mxDestroyArray(return_val);
}
