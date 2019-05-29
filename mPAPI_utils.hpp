#ifndef _MPAPI_STATUS_H_
#define _MPAPI_STATUS_H_

#include <algorithm>
#include <vector>
#include <string>
#include <mex.h>
#include <papi.h>

mxArray *mPAPI_create_int32_scalar(int value)
{
    mxArray *mx_value = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    ((int *)mxGetPr(mx_value))[0] = value;
    return mx_value;
}

int mPAPI_get_int32_scalar(mxArray *array)
{
    return *((int *)mxGetData(array));
}

void mPAPI_remove_event_code(std::vector<int> &event_codes, int event_code_to_remove)
{
    event_codes.erase(
        std::find(event_codes.begin(),
                  event_codes.end(),
                  event_code_to_remove));
}

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

void mPAPI_mex_error_with_reason(std::string base_error_msg, int reason_id)
{
    std::string reason_msg = PAPI_strerror(reason_id);
    base_error_msg += ". Reason: ";
    base_error_msg += reason_msg;
    mexErrMsgTxt(base_error_msg.c_str());
}

void mPAPI_mex_warn_with_reason(std::string base_error_msg, int reason_id)
{
    std::string reason_msg = PAPI_strerror(reason_id);
    base_error_msg += ". Reason: ";
    base_error_msg += reason_msg;
    mexWarnMsgTxt(base_error_msg.c_str());
}

mxArray *mPAPI_to_string_cell_array(std::vector<std::string> names)
{
    mxArray *native_events = mxCreateCellMatrix(1, names.size());
    for (int i = 0; i < names.size(); ++i)
    {
        mxArray *name = mxCreateString(names[i].c_str());
        mxSetCell(native_events, i, mxDuplicateArray(name));
        mxDestroyArray(name);
    }
    return native_events;
}

void mPAPI_event_names_cell_to_codes(const mxArray *names_in_cell, int num_events, std::vector<int> &out_codes, std::vector<std::string> &out_names)
{
    if (!mxIsCell(names_in_cell))
    {
        mexErrMsgTxt("Argument is not a cell array.");
    }

    for (int i = 0; i < num_events; ++i)
    {
        mxArray *event_name_ptr = mxGetCell(names_in_cell, i);
        if (mxIsChar(event_name_ptr))
        {
            char *event_name = mxArrayToString(event_name_ptr);
            int event_code;
            int retval;
            if ((retval = PAPI_event_name_to_code(event_name, &event_code)) != PAPI_OK)
            {
                mPAPI_mex_warn_with_reason("Failed to convert event name to code", retval);
            }
            else
            {
                out_codes.push_back(event_code);
                out_names.push_back(event_name);
            }
        }
    }
}

/* _MPAPI_STATUS_H_ */
#endif
