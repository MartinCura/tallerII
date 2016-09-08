FROM  gcc:4.9

RUN apt-get update
RUN apt-get install cmake -y

WORKDIR /home

ADD src/ /home/src

RUN cmake src/
RUN make
RUN make test

CMD  ./ApplicationServer_src