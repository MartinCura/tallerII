# ApplicationServer

Para correr el programa y evitar que se generen archivos de CMake no deseados

```
mkdir ../build
cd ../build
cmake .. && make
./ApplicationServer_src

Para generar un paquete .deb:

```
/tallerII/ApplicationServer$  cmake code/
/tallerII/ApplicationServer$  make package
```

Para instalar el paquete (que resultara en el ejecutable ubicado en /usr/appserver):

```
/tallerII/ApplicationServer$  sudo dpkg -i ApplicationServer-0.1.1-Linux.deb
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

