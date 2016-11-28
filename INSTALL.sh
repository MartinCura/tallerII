#!/bin/bash
#Installing AppServer

sudo rm -rf /usr/appServer
sudo rm -rf build
sudo mkdir build
cd build
sudo cmake ..
sudo make

