#include "mex.h"
#include "matrix.h"
#include "papi.h"

#include "MAPI_status.h"

/* PAPI_tic.c - start counters */
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
    if (nrhs != 1) {
        mexErrMsgTxt("MAPI_tic requires one vector with counter descriptors.");
    }

    /* Check counter descriptors */
    unsigned int mrows = mxGetM(prhs[0]);
    unsigned int ncols = mxGetN(prhs[0]);
    if (!mxIsInt32(prhs[0]) || mrows > 1 || ncols == 0) {
        mexErrMsgTxt("MAPI_tic requries a non-empty row vector with counter descriptors (int32).");
    }
    
    if (MAPI_getGlobalStatus() != MAPI_STATUS_COUNTING) {
        int* descriptors = mxGetData(prhs[0]);
        int result;
        if ((result = PAPI_start_counters(descriptors, ncols)) != PAPI_OK) {
            char error[128];
            sprintf(error, "MAPI_tic couldn't start counters. Error message: %s", PAPI_strerror(result));
            mexErrMsgTxt(error);
        }
        MAPI_setGlobalStatus(MAPI_STATUS_COUNTING);
    } else {
        mexPrintf("Counters are already running. Execute MAPI_toc instead.\n");
    }
}
