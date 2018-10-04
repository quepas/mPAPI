#ifndef _MPAPI_STATUS_H_
#define _MPAPI_STATUS_H_

#include "mex.h"
#include <string.h>

void mPAPI_set_event_set(int event_set_id)
{
    mxArray *variable = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    int *value = (int *)mxGetData(variable);
    *value = event_set_id;
    mexPutVariable("global", "mPAPI_event_set", variable);
}

int mPAPI_get_event_set()
{
    mxArray *variable = mexGetVariable("global", "mPAPI_event_set");
    return *((int *)mxGetData(variable));
}

/* _MPAPI_STATUS_H_ */
#endif
