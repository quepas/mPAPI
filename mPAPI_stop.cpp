#include <papi.h>
#include <mex.h>
#include "mPAPI_utils.hpp"

void handler(int EventSet, void *address, long_long overflow_vector, void *context)
{
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
   int event_set = mPAPI_get_int32_scalar(mxGetFieldByNumber(prhs[0], 0, 0));
   int overflow_event_code = mPAPI_get_int32_scalar(mxGetFieldByNumber(prhs[0], 0, 2));
   int num_events = mPAPI_get_int32_scalar(mxGetFieldByNumber(prhs[0], 0, 4));

   int retval;
   long long values[num_events];

   if ((retval = PAPI_stop(event_set, values)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to stop the event counting", retval);
   }

   /* clear the overflow status */
   if ((retval = PAPI_overflow(event_set, overflow_event_code, 0, 0,
                               handler)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to stop the sampling with overflow", retval);
   }
   if ((retval = PAPI_cleanup_eventset(event_set)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to cleanup the event set", retval);
   }
   /* Free all memory and data structures, EventSet must be empty. */
   if ((retval = PAPI_destroy_eventset(&event_set)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to destroy the event set", retval);
   }
   /* free the resources used by PAPI */
   PAPI_shutdown();
}