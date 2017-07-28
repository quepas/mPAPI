#include "mex.h"
#include "matrix.h"
#include "papi.h"

#include "MAPI_status.h"
/*
 *  MAPI_create
 */
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
    int papi_num_counters = PAPI_num_counters();
    
    if (nrhs < 1 || nrhs > papi_num_counters) {
        char error[128];
        sprintf(error, "MAPI_create takes from 1 to %d "
                    "counter descriptors (up to number of available "
                    "hardware performance monitoring counters)", papi_num_counters);
        mexErrMsgTxt(error);
    }

    /* Stop all running counters */
    if (MAPI_getGlobalStatus() == MAPI_STATUS_COUNTING) {
        mexPrintf("Stopping all current counters");
        PAPI_stop_counters(NULL, 0);
    }

    /* Set new global state */
    MAPI_setGlobalStatus(MAPI_STATUS_INIT);

    /* Create an event code vector */
    int* events = mxCalloc(nrhs, sizeof(INT32_T));

    /* Set an output vector */
    plhs[0] = mxCreateNumericMatrix(1, nrhs, mxINT32_CLASS, mxREAL);
    int* out_vector = mxGetData(plhs[0]);

    int i;
    for (i = 0; i < nrhs; ++i) {
        if (!mxIsChar(prhs[i])) {
            mexErrMsgTxt("MAPI_create requires a string with hardware counter name.");
        }

        size_t id_length = mxGetNumberOfElements(prhs[i]);
        char buffer[256];
        if (mxGetString(prhs[i], buffer, id_length+1) == 1) {
            mexErrMsgTxt("Error while reading a string name.");
        }

        int result;
        int event;
        if (result = PAPI_event_name_to_code(buffer, &(event)) != PAPI_OK) {
            char error[256];
            sprintf(error, "Failed to obtained event code %s."
                           "Error message: %s.", buffer, PAPI_strerror(result));
            mexErrMsgTxt(error);
        }
        out_vector[i] = event;
    }
}
