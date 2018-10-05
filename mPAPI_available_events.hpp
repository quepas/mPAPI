#ifndef _MPAPI_AVAILABLE_EVENTS_
#define _MPAPI_AVAILABLE_EVENTS_

#include <algorithm>
#include <map>
#include <vector>
#include <papi.h>
#include "mPAPI_utils.hpp"

// Function definitions
void fillOneEventSet(std::multimap<int, int> preset_max_avail,
                     std::vector<int> &event_codes,
                     std::vector<int> &event_set_codes)
{
    int retval;
    int event_set = PAPI_NULL;
    if ((retval = PAPI_create_eventset(&event_set)) != PAPI_OK)
    {
        mPAPI_mex_error_with_reason("Failed to create an event set", retval);
    }
    int max_counters = PAPI_num_counters();
    int in_event_set = 1;
    for (auto &pair : preset_max_avail)
    {
        int event_id = pair.second;
        PAPI_event_info_t info;
        PAPI_get_event_info(event_id, &info);
        // Any space left in the EventSet?
        if (in_event_set > max_counters)
            break;
        if (PAPI_add_event(event_set, event_id) == PAPI_OK)
        {
            mPAPI_remove_event_code(event_codes, event_id);
            event_set_codes.push_back(event_id);
            in_event_set++;
        }
    }
}

void countAvailability(std::vector<int> event_codes,
                       std::multimap<int, int> &preset_max_avail)
{
    int retval;
    int event_set = PAPI_NULL;
    if ((retval = PAPI_create_eventset(&event_set)) != PAPI_OK)
    {
        mPAPI_mex_error_with_reason("Failed creating an event set", retval);
    }
    // Check each event
    for (int i = 0; i < event_codes.size(); ++i)
    {
        PAPI_add_event(event_set, event_codes[i]);
        // By looking how many other events are compatible with it
        for (int k = i + 1; k < event_codes.size(); ++k)
        {
            PAPI_add_event(event_set, event_codes[k]);
        }
        // Check how many is compatible
        int availableSize = PAPI_num_events(event_set);
        PAPI_cleanup_eventset(event_set);
        preset_max_avail.insert({availableSize, event_codes[i]});
    }
}

// ~_AVAILABLE_EVENTS_
#endif