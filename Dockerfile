FROM ubuntu:14.04

RUN apt-get update \
    && apt-get install -y \
    cmake \
    build-essential \
    gcc \
    make \
    python3.4 \
    python3.4-dev \
    python-pycurl \
    python-pip \
    && apt-get autoremove \
    && apt-get clean

RUN pip install pyresttest

RUN mkdir /application
RUN mkdir /application/build


WORKDIR /application/build
COPY . /application

RUN cmake ..
RUN make

RUN ./ApplicationServer_test;

COPY /ApplicationServer/img /usr/appServer
COPY /ApplicationServer/src/config.js /usr/appServer

CMD ./ApplicationServer_src 

