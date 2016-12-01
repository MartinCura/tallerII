#!/bin/bash
#Runs unit tests

sudo rm -rf build
sudo mkdir build
cd build
sudo cmake ..
sudo make
sudo ./ApplicationServer_test

