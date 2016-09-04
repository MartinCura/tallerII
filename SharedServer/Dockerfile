FROM node 

RUN apt-get update && apt-get install vim postgresql-client -y

RUN npm install -g nodemon
WORKDIR /home

#RUN mkdir /home/deploy

COPY code/package.json /home/package.json


RUN npm set progress=false
# RUN npm install

RUN npm install

RUN cat package.json


# CMD ["nodemon" , "-L", "server.js"]
COPY entrypoint.sh /home/entrypoint.sh
ENTRYPOINT ./entrypoint.sh

# el entrypoint tiene que mover esto, sino cuando se crea el volumen lo va a pisar
#CMD ["tail" , "-f", "/dev/null"]

