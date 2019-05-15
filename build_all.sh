#!/bin/sh
./build_matlab.sh '/usr/local/MATLAB/R2013a/bin/mex' '/usr/bin/g++-4.4' 'r2013a/'
./build_matlab.sh '/usr/local/MATLAB/R2015b/bin/mex' '/usr/bin/g++-4.7' 'r2015b/'
./build_matlab.sh '/usr/local/MATLAB/R2018b/bin/mex' '/usr/bin/g++-6'   'r2018b/'

# Octave 4.2.2
./build_octave.sh '/usr/bin/mkoctfile' '/usr/bin/g++-6' 'octave422/'
# Octave 5.1.0
./build_octave.sh '/home/quepas/PhD/Tools/Octave/octave-5.1.0/src/mkoctfile' '/usr/bin/g++-6' 'octave510/'

# Copy common-matlab to each output directories
cp -a common-matlab/. r2013a/
cp -a common-matlab/. r2015b/
cp -a common-matlab/. r2018b/
cp -a common-matlab/. octave422/
cp -a common-matlab/. octave510/
