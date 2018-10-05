#!/bin/sh
MEX=$1
COMPILER=$2
OUT_DIR=$3
$MEX CXX=$COMPILER -I/usr/local/include mPAPI_groupEvents.cpp     -L/usr/local/lib/ -lpapi -output mPAPI_groupEvents -outdir $OUT_DIR
$MEX CXX=$COMPILER -I/usr/local/include mPAPI_enumPresetEvent.cpp -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_enumPresetEvent -outdir $OUT_DIR
$MEX CXX=$COMPILER -I/usr/local/include mPAPI_enumNativeEvent.cpp -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_enumNativeEvent -outdir $OUT_DIR
$MEX CXX=$COMPILER -I/usr/local/include mPAPI_register.cpp        -L/usr/local/lib/ -lpapi -output mPAPI_register -outdir $OUT_DIR
$MEX CXX=$COMPILER -I/usr/local/include mPAPI_tic.cpp             -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_tic -outdir $OUT_DIR
$MEX CXX=$COMPILER -I/usr/local/include mPAPI_toc.cpp             -L/usr/local/lib/ -lpapi -output $OUT_DIR/mPAPI_toc -outdir $OUT_DIR
