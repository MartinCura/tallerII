# ApplicationServer

Para correr el programa y evitar que se generen archivos de CMake no deseados

```
mkdir ../build
cd ../build
cmake .. && make
./ApplicationServer_src
```

Para levantar un contenerdor de Docker (luego se va a poder acceder en http://127.0.0.1:8000):

```
/tallerII/ApplicationServer$  sudo docker build -t appserverimg .
/tallerII/ApplicationServer$  sudo docker run --name appserver -p 8000:8000 appserverimg
```

Para detenerlo:

```
sudo docker stop appserver
```

Para eliminarlo:

```
sudo docker rm appserver
```

Para instalar leveldb:

```
sudo apt-get install libsnappy-dev
git clone https://github.com/google/leveldb.git
cd leveldb
make
sudo mv out-shared/libleveldb.* /usr/local/lib
sudo mv out-static/libleveldb.* /usr/local/lib
sudo cp -R include/leveldb /usr/local/include
```
