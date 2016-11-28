#!/bin/bash
#Creates the coverage report

sudo rm -rf build
sudo mkdir build
cd build
sudo cmake ..
sudo make coverage_target

