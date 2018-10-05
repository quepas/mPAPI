#include <vector>
#include <string>
#include <mex.h>
#include <matrix.h>
#include <papi.h>
#include "mPAPI_utils.hpp"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
   if (!PAPI_is_initialized())
      PAPI_library_init(PAPI_VER_CURRENT);

   int retval;
   int event_id = 0 | PAPI_NATIVE_MASK;

   /* For consistency, always ASK FOR the first event, if there is not one then nothing to process */
   if ((retval = PAPI_enum_event(&event_id, PAPI_ENUM_FIRST)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to enumerate first performance event.", retval);
   }

   int event_set = PAPI_NULL;
   // Create an event set for testing enumarated native events
   // PAPI_query_event() fails to asses if an event is really usable on the platform
   if ((retval = PAPI_create_eventset(&event_set)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to create an event set.", retval);
   }

   std::vector<std::string> event_names;
   do
   {
      if (IS_NATIVE(event_id))
      {
         char event_code[PAPI_MAX_STR_LEN];
         if ((retval = PAPI_event_code_to_name(event_id, event_code)) != PAPI_OK)
         {
            mPAPI_mex_warn_with_reason("Failed to convert event code to name.", retval);
         }
         // Add to a event set just for fun ^^
         if (PAPI_add_named_event(event_set, event_code) == PAPI_OK)
         {
            event_names.push_back(event_code);
            PAPI_cleanup_eventset(event_set);
         }
      }
   } while (PAPI_enum_event(&event_id, PAPI_ENUM_ALL) == PAPI_OK);

   mxArray *native_events = mPAPI_to_string_cell_array(event_names);
   plhs[0] = mxDuplicateArray(native_events);
   mxDestroyArray(native_events);
}
