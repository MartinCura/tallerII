#!/bin/bash

docker build -t appserverimg ../..;
docker run --name appserver -p 8000:8000 appserverimg &

docker build -t testingappserverimg .;
docker run --name testingappserver --link appserver:appserverhost -ti testingappserverimg;

docker stop appserver;
docker stop testingappserver;

sudo docker rm appserver;
sudo docker rm testingappserver;
sudo docker rmi appserverimg;
sudo docker rmi testingappserverimg;
