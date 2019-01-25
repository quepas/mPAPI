#!/bin/sh

# Install PAPI
sudo apt-get update
sudo apt-get install papi-tools
sudo apt-get install libpapi-dev

# Install old g++ releases (4.4, 4.7, 5)
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo add-apt-repository ppa:jonathonf/g++-6.3
sudo apt-get update
sudo apt-get install g++-4.4
sudo apt-get install g++-4.7
sudo apt-get install g++-6
