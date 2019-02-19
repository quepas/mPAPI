#!/bin/sh
./build.sh '/usr/local/MATLAB/R2013a/bin/mex' '/usr/bin/g++-4.4' 'r2013a/'
./build.sh '/usr/local/MATLAB/R2015b/bin/mex' '/usr/bin/g++-4.7' 'r2015b/'
./build.sh '/usr/local/MATLAB/R2018b/bin/mex' '/usr/bin/g++-6'   'r2018b/'

# Copy common-matlab to each output directories
cp -a common-matlab/. r2013a/
cp -a common-matlab/. r2015b/
cp -a common-matlab/. r2018b/
