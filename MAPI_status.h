#ifndef _MAPI_STATUS_H_
#define _MAPI_STATUS_H_

#include "mex.h"

#define MAPI_STATUS_NONE -2
#define MAPI_STATUS_INIT -1
#define MAPI_STATUS_COUNTING 0
/* MAPI_STATUS_1, 2, 3,... number of active counters */

/*
 *  Set global variable MAPI_status
 */
void MAPI_setGlobalStatus(int initial_value) {
    mxArray* MAPI_status = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    int* status = (int*)mxGetData(MAPI_status);
    *status = initial_value;
    mexPutVariable("global", "MAPI_status", MAPI_status);    
}

/*
 *  Get global variable MAPI_status
 */
int MAPI_getGlobalStatus() {
    mxArray* MAPI_status = mexGetVariable("global", "MAPI_status");
    if (MAPI_status == 0) {
        return MAPI_STATUS_NONE;
    }
    return *((int*)mxGetData(MAPI_status));
}

int MAPI_getNumRunningCounters() {
    int current_status = MAPI_getGlobalStatus();
    if (current_status > MAPI_STATUS_COUNTING) {
        return current_status;
    }
    return 0;
}

/* _MAPI_STATUS_H_ */
#endif
