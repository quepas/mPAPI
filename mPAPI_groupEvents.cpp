#include <map>
#include <vector>
#include <mex.h>
#include <matrix.h>
#include <papi.h>
#include "mPAPI_utils.hpp"
#include "mPAPI_available_events.hpp"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (!PAPI_is_initialized())
        PAPI_library_init(PAPI_VER_CURRENT);

    if (!mexIsLocked())
    {
        mexLock();
    }

    size_t num_events = mxGetNumberOfElements(prhs[0]);
    int max_counters = PAPI_num_counters();
    std::vector<int> event_codes;
    mPAPI_event_names_cell_to_codes(prhs[0], num_events, event_codes);

    std::vector<mxArray *> event_groups;
    while (!event_codes.empty())
    {
        std::multimap<int, int> map_event_avail;
        countAvailability(event_codes, map_event_avail);
        std::vector<int> event_set_codes;
        fillOneEventSet(map_event_avail, event_codes, event_set_codes);

        mxArray *groupCell = mxCreateCellMatrix(1, event_set_codes.size());
        for (int i = 0; i < event_set_codes.size(); ++i)
        {
            char code[PAPI_2MAX_STR_LEN];
            PAPI_event_code_to_name(event_set_codes[i], code);
            mxArray *str = mxCreateString(code);
            mxSetCell(groupCell, i, mxDuplicateArray(str));
            mxDestroyArray(str);
        }
        event_groups.push_back(mxDuplicateArray(groupCell));
        mxDestroyArray(groupCell);
    }
    mxArray *cell_event_groups = mxCreateCellMatrix(1, event_groups.size());
    for (int i = 0; i < event_groups.size(); ++i)
    {
        mxSetCell(cell_event_groups, i, mxDuplicateArray(event_groups[i]));
        mxDestroyArray(event_groups[i]);
    }
    plhs[0] = mxDuplicateArray(cell_event_groups);
    mxDestroyArray(cell_event_groups);
}
