#!/bin/bash
#Creates a Debian package

sudo rm -rf build
sudo mkdir build
cd build
sudo cmake ..
sudo make package

