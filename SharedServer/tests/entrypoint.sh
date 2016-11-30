#!/bin/sh

export HOSTADD=`/sbin/ip route|awk '/default/ { print $3 }'`

cd /home
echo $HOSTADD
resttest.py http://$HOSTADD integration.yaml