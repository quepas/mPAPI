#ifndef _MPAPI_STATUS_H_
#define _MPAPI_STATUS_H_

#include "mex.h"

#define MPAPI_STATUS_NONE -2
#define MPAPI_STATUS_INIT -1
#define MPAPI_STATUS_COUNTING 0
/* MPAPI_STATUS_1, 2, 3,... number of active counters */

/*
 *  Set global variable mPAPI_status
 */
void mPAPI_setGlobalStatus(int initial_value)
{
    mxArray *mPAPI_status = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    int *status = (int *)mxGetData(mPAPI_status);
    *status = initial_value;
    mexPutVariable("global", "mPAPI_status", mPAPI_status);
}

/*
 *  Get global variable MAPI_status
 */
int mPAPI_getGlobalStatus()
{
    mxArray *mPAPI_status = mexGetVariable("global", "mPAPI_status");
    if (mPAPI_status == 0)
    {
        return MPAPI_STATUS_NONE;
    }
    return *((int *)mxGetData(mPAPI_status));
}

int mPAPI_getNumRunningCounters()
{
    int current_status = mPAPI_getGlobalStatus();
    if (current_status > MPAPI_STATUS_COUNTING)
    {
        return current_status;
    }
    return 0;
}

/* _MPAPI_STATUS_H_ */
#endif
