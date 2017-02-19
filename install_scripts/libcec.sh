#!/bin/bash

echo "Dependencies"
sudo apt-get update
sudo apt-get install cmake libudev-dev libxrandr-dev python-dev swig
cd
echo "Platform"
git clone https://github.com/Pulse-Eight/platform.git
mkdir platform/build
cd platform/build
cmake ..
make
sudo make install
cd
echo "libcec"
git clone https://github.com/Pulse-Eight/libcec.git
mkdir libcec/build
cd libcec/build
cmake -DRPI_INCLUDE_DIR=/opt/vc/include -DRPI_LIB_DIR=/opt/vc/lib ..
make -j4
echo "Install (sudo is calles from script)"
sudo make install
sudo ldconfig
