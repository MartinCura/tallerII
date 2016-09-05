#!/bin/bash

sudo docker build -t appserverimg ..;
sudo docker run --name appserver -p 8000:8000 appserverimg &

sudo docker build -t testingappserverimg .;
sudo docker run --name testingappserver --link appserver:appserverhost -ti testingappserverimg;

sudo docker stop appserver;
sudo docker stop testingappserver;

#sudo docker rm appserver;
#sudo docker rm testingappserver;
#sudo docker rmi appserverimg;
#sudo docker rmi testingappserverimg;
