#include "mex.h"
#include "matrix.h"
#include "papi.h"

#include "MAPI_status.h"

/* PAPI_toc.c - start counters */
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
    if (nrhs < 1) {
        mexErrMsgTxt("MAPI_toc requires one vector with counter descriptors.");
    }
    if (MAPI_getGlobalStatus() != MAPI_STATUS_COUNTING) {
        mexErrMsgTxt("No active counters. Run MAPI_tic before.");
    }

    /* Check counter descriptors */
    unsigned int mrows = mxGetM(prhs[0]);
    unsigned int ncols = mxGetN(prhs[0]);
    if (!mxIsInt32(prhs[0]) || mrows > 1 || ncols == 0) {
        mexErrMsgTxt("MAPI_toc requries a non-empty row vector "
                     "with counter descriptors (int32).");
    }

    plhs[0] = mxCreateNumericMatrix(1, ncols, mxINT64_CLASS, mxREAL);
    long long *counter_values = mxGetData(plhs[0]);
    int result;
    if ((result = PAPI_read_counters(counter_values, ncols)) != PAPI_OK) {
        char error[256];
        sprintf(error, "Failed to read counters. "
                       "Error message: %s.", PAPI_strerror(result));
    }
}
