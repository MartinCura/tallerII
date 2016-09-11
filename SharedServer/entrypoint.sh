#!/bin/sh

mkdir /home/code
cp -rf /home/src/* /home/code/
cd /home/code
psql -f db.sql
nodemon server.js
tail -f /dev/null