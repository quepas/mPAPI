cmake_minimum_required(VERSION 3.14)
project(mPAPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(Matlab_ROOT_DIR ${MATLAB_PATH})
set(BUILD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${BUILD_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_PATH})

find_package(PAPI)
find_package(Matlab)

include_directories(. ${Matlab_INCLUDE_DIRS} ${PAPI_INCLUDE_DIRS})
link_directories(${PAPI_LIBRARY_DIRS})

matlab_add_mex(NAME mPAPI_register SRC mPAPI_register.cpp LINK_TO papi)
matlab_add_mex(NAME mPAPI_toc SRC mPAPI_toc.cpp LINK_TO papi)
matlab_add_mex(NAME mPAPI_tic SRC mPAPI_tic.cpp LINK_TO papi)
matlab_add_mex(NAME mPAPI_trace_register SRC mPAPI_trace_register.cpp LINK_TO papi)
matlab_add_mex(NAME mPAPI_trace_toc SRC mPAPI_trace_toc.cpp LINK_TO papi)
matlab_add_mex(NAME mPAPI_trace_tic SRC mPAPI_trace_tic.cpp LINK_TO papi)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/common-matlab/findCompThreads.m
        ${CMAKE_CURRENT_SOURCE_DIR}/common-matlab/findMaxCompThreads.m
        ${CMAKE_CURRENT_SOURCE_DIR}/common-matlab/pidOf.m
        ${CMAKE_CURRENT_SOURCE_DIR}/common-matlab/threadsOf.m
        DESTINATION ${BUILD_PATH})
