#include <vector>
#include <string>

#include "mPAPI_status.h"
#include "mex.h"
#include "matrix.h"
#include "papi.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
   if (!PAPI_is_initialized())
      PAPI_library_init(PAPI_VER_CURRENT);

   int retval;
   int event_id = 0 | PAPI_PRESET_MASK;

   /* For consistency, always ASK FOR the first event, if there is not one then nothing to process */
   if (PAPI_enum_event(&event_id, PAPI_ENUM_FIRST) != PAPI_OK)
   {
      mexErrMsgTxt("Failed to enumerate first performance event.");
   }

   std::vector<std::string> event_names;
   do
   {
      char event_code[PAPI_MIN_STR_LEN];
      if ((retval = PAPI_event_code_to_name(event_id, event_code)) != PAPI_OK)
      {
         mexWarnMsgTxt("Failed to convert event code to name.");
      }
      else
      {
         event_names.push_back(event_code);
      }
   } while (PAPI_enum_event(&event_id, PAPI_PRESET_ENUM_AVAIL) == PAPI_OK);

   mxArray *preset_events = mPAPI_to_string_cell_array(event_names);
   plhs[0] = mxDuplicateArray(preset_events);
   mxDestroyArray(preset_events);
}
