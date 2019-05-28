#!/bin/sh
MEX=$1
COMPILER=$2
OUT_DIR=$3
$MEX CXX=$COMPILER CC=$COMPILER GCC=$COMPILER CXXFLAGS='$CXXFLAGS -std=c++0x' -I/usr/local/include mPAPI_groupEvents.cpp     -L/usr/local/lib/ -lpapi -output mPAPI_groupEvents -outdir $OUT_DIR
$MEX CXX=$COMPILER CC=$COMPILER GCC=$COMPILER CXXFLAGS='$CXXFLAGS -std=c++0x' -I/usr/local/include mPAPI_enumPresetEvents.cpp -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_enumPresetEvents -outdir $OUT_DIR
$MEX CXX=$COMPILER CC=$COMPILER GCC=$COMPILER CXXFLAGS='$CXXFLAGS -std=c++0x' -I/usr/local/include mPAPI_enumNativeEvents.cpp -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_enumNativeEvents -outdir $OUT_DIR
$MEX CXX=$COMPILER CC=$COMPILER GCC=$COMPILER CXXFLAGS='$CXXFLAGS -std=c++0x' -I/usr/local/include mPAPI_register.cpp        -L/usr/local/lib/ -lpapi -output mPAPI_register -outdir $OUT_DIR
$MEX CXX=$COMPILER CC=$COMPILER GCC=$COMPILER CXXFLAGS='$CXXFLAGS -std=c++0x' -I/usr/local/include mPAPI_tic.cpp             -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_tic -outdir $OUT_DIR
$MEX CXX=$COMPILER CC=$COMPILER GCC=$COMPILER CXXFLAGS='$CXXFLAGS -std=c++0x' -I/usr/local/include mPAPI_toc.cpp             -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_toc -outdir $OUT_DIR
# Traces
$MEX CXX=$COMPILER CC=$COMPILER GCC=$COMPILER CXXFLAGS='$CXXFLAGS -std=c++0x' -I/usr/local/include mPAPI_trace_register.cpp             -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_trace_register -outdir $OUT_DIR
$MEX CXX=$COMPILER CC=$COMPILER GCC=$COMPILER CXXFLAGS='$CXXFLAGS -std=c++0x' -I/usr/local/include mPAPI_trace_tic.cpp             -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_trace_tic -outdir $OUT_DIR
$MEX CXX=$COMPILER CC=$COMPILER GCC=$COMPILER CXXFLAGS='$CXXFLAGS -std=c++0x' -I/usr/local/include mPAPI_trace_toc.cpp             -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_trace_toc -outdir $OUT_DIR
