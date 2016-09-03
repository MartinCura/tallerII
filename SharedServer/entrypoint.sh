#!/bin/sh

mkdir /home/code
cp -rf /home/src/* /home/code/
cd /home/code
nodemon server.js
tail -f /dev/null