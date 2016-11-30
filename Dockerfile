FROM  gcc:4.9

RUN apt-get update && apt-get install cmake -y

RUN mkdir /application
RUN mkdir /application/build


WORKDIR /application/build
COPY . /application

RUN cmake ..
RUN make

RUN ./ApplicationServer_test;

CMD ./ApplicationServer_src 

