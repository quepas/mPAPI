#!/bin/sh
MEX=$1
COMPILER=$2
OUT_DIR=$3

mkdir -p $OUT_DIR
cd $OUT_DIR

export CXX=$COMPILER
export CXXFLAGS='-std=c++0x'

$MEX --mex -I/usr/local/include ../mPAPI_groupEvents.cpp -L/usr/local/lib/ -lpapi -output mPAPI_groupEvents
$MEX --mex -I/usr/local/include ../mPAPI_enumPresetEvents.cpp -L/usr/local/lib/ -lpapi -output mPAPI_enumPresetEvents
$MEX --mex -I/usr/local/include ../mPAPI_enumNativeEvents.cpp -L/usr/local/lib/ -lpapi -output mPAPI_enumNativeEvents
$MEX --mex -I/usr/local/include ../mPAPI_register.cpp -L/usr/local/lib/ -lpapi -output mPAPI_register
$MEX --mex -I/usr/local/include ../mPAPI_tic.cpp -L/usr/local/lib/ -lpapi -output mPAPI_tic
$MEX --mex -I/usr/local/include ../mPAPI_toc.cpp -L/usr/local/lib/ -lpapi -output mPAPI_toc
