FROM python:3

RUN apt-get update && apt-get install -y python-pycurl python-pip

RUN pip install pyresttest

COPY integration.yaml /home
COPY entrypoint.sh /home

ENTRYPOINT /home/entrypoint.sh

