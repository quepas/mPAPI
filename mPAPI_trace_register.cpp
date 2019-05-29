#include <mex.h>
#include <papi.h>
#include <iostream>
#include <fstream>
#include "mPAPI_utils.hpp"

std::ofstream trace_file;
int num_events;

std::string prepare_trace_file_header(std::vector<std::string> event_names)
{
   std::string str;
   for (int k = 0; k < event_names.size(); ++k)
   {
      str += event_names[k];
      if (k + 1 != event_names.size())
      {
         str += ",";
      }
   }
   return str;
}

void handler(int EventSet, void *address, long_long overflow_vector, void *context)
{
   long long counter_values[num_events];
   int retval;
   if ((retval = PAPI_read(EventSet, counter_values)) == PAPI_OK)
   {
      switch (num_events)
      {
      case 1:
         trace_file << counter_values[0] << std::endl;
         break;
      case 2:
         trace_file << counter_values[0] << ","
                    << counter_values[1] << std::endl;
         break;
      case 3:
         trace_file << counter_values[0] << ","
                    << counter_values[1] << ","
                    << counter_values[2] << std::endl;
         break;
      case 4:
         trace_file << counter_values[0] << ","
                    << counter_values[1] << ","
                    << counter_values[2] << ","
                    << counter_values[3] << std::endl;
         break;
      case 5:
         trace_file << counter_values[0] << ","
                    << counter_values[1] << ","
                    << counter_values[2] << ","
                    << counter_values[3] << ","
                    << counter_values[4] << std::endl;
         break;
      default:
         break;
      }
   }
}

/*
 *  mPAPI_trace_register -- registers new trace (overflow and measured events, overflow threshold)
 *  Parameters:
 *      * overflow_event: just one event name
 *      * overflow_threshold: a number
 *      * performance_events : cell array -- stores the names of preset or native events to measure
 *      * trace file: string
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
   std::string trace_file_name = mxArrayToString(prhs[3]);
   trace_file.open(trace_file_name, std::fstream::app);

   if (!PAPI_is_initialized())
      PAPI_library_init(PAPI_VER_CURRENT);

   /**
    * Create an event set
    */
   int event_set = PAPI_NULL;
   int retval;
   if ((retval = PAPI_create_eventset(&event_set)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to create an event set.", retval);
   }
   // Count for children as well (important for multi-thread execution)
   /*if (retval = PAPI_assign_eventset_component(event_set, 0) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to assign an event set to the 0-component.", retval);
   }
   PAPI_option_t opt;
   std::memset(&opt, 0x0, sizeof(PAPI_option_t));
   opt.inherit.inherit = PAPI_INHERIT_ALL;
   opt.inherit.eventset = event_set;
   if ((retval = PAPI_set_opt(PAPI_INHERIT, &opt)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to inherit events counting to child processes.", retval);
   }*/
   // Add all events (including the overflow event)
   size_t num_measure_events = mxGetNumberOfElements(prhs[2]);
   std::vector<int> event_codes;
   std::vector<std::string> event_names;
   mPAPI_event_names_cell_to_codes(prhs[2], num_measure_events, event_codes, event_names);
   // Decode the overflow event
   char *event_name = mxArrayToString(prhs[0]);
   event_names.insert(event_names.begin(), event_name);
   int overflow_event;
   if ((retval = PAPI_event_name_to_code(event_name, &overflow_event)) != PAPI_OK)
   {
      mPAPI_mex_warn_with_reason("Failed to convert event name to code", retval);
   }
   // All events to the set
   event_codes.insert(event_codes.begin(), overflow_event);
   num_events = event_codes.size();
   int *counters = (int *)mxCalloc(num_events, sizeof(int));
   for (int i = 0; i < num_events; ++i)
   {
      counters[i] = event_codes[i];
   }
   if ((retval = PAPI_add_events(event_set, counters, num_events)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to add performance events to the event set.", retval);
   }
   mxFree(counters);
   /**
    * Start profiling with event overflow
    */
   // Decode the threshold value
   int threshold_value = (int)mxGetScalar(prhs[1]);
   if ((retval = PAPI_overflow(event_set, overflow_event, threshold_value, 0, handler)) != PAPI_OK)
   {
      mPAPI_mex_error_with_reason("Failed to start the profiling.", retval);
   }

   /* Start counting */
   // if ((retval = PAPI_start(event_set)) != PAPI_OK)
   //    mPAPI_mex_error_with_reason("Failed to start the profiling.", retval);

   // The output is a structure with fields:
   //    1) Id of the event set
   //    2) Name of the overflow event
   //    3) Code of the overflow event
   //    4) The threshold of the overflow sampling
   //    5) Number of measured events (including the threshold event)
   mwSize dims[2] = {1, 1};
   const char *field_names[] = {
       "event_set",
       "overflow_event_name",
       "overflow_event_code",
       "perf_events",
       "threshold_value",
       "num_events",
       "trace_file"};
   mxArray *profileStruct = mxCreateStructArray(2, dims, 7, field_names);
   mxSetFieldByNumber(profileStruct, 0, 0, mPAPI_create_int32_scalar(event_set));
   mxSetFieldByNumber(profileStruct, 0, 1, mxCreateString(event_name));
   mxSetFieldByNumber(profileStruct, 0, 2, mPAPI_create_int32_scalar(overflow_event));
   mxSetFieldByNumber(profileStruct, 0, 3, mxCreateString(prepare_trace_file_header(event_names).c_str()));
   mxSetFieldByNumber(profileStruct, 0, 4, mPAPI_create_int32_scalar(threshold_value));
   mxSetFieldByNumber(profileStruct, 0, 5, mPAPI_create_int32_scalar(num_events));
   mxSetFieldByNumber(profileStruct, 0, 6, mxCreateString(trace_file_name.c_str()));
   plhs[0] = mxDuplicateArray(profileStruct);

   mxDestroyArray(profileStruct);
}
