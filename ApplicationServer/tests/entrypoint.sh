#!/bin/sh

resttest.py http://appserverhost:8000 /home/testing_allusers.yaml
resttest.py http://appserverhost:8000 /home/testing_users.yaml
resttest.py http://appserverhost:8000 /home/testing_contacts.yaml
resttest.py http://appserverhost:8000 /home/testing_recommendations.yaml
