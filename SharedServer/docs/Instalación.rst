Instalación y deploy
====================

Correr en ambiente de test
--------------------------

Es necesario contar con `Docker <https://www.docker.com/>`_ y `Docker Compose <https://docs.docker.com/compose/>`_ instalados en el sistema. Instalación sin estos componenetes de virtualización es posible, pero no se detalla en esta lista.

Para inicializar el servidor, deberemos ejecutar el siguiente comando:

``$ sudo docker-compose build``

Y luego para inicializar el servidor:

``$ sudo docker-compose up``

La salida esperada debería ser la siguiente. Dependiendo de su sistema, su salida podría ser distinta. ::

   Starting sharedserver_db_1
   Recreating sharedserver_sharedserver_1
   Attaching to sharedserver_db_1, sharedserver_sharedserver_1
   db_1            | LOG:  database system was shut down at 2016-10-30 16:04:01 UTC
   db_1            | LOG:  MultiXact member wraparound protections are now enabled
   db_1            | LOG:  database system is ready to accept connections
   db_1            | LOG:  autovacuum launcher started
   sharedserver_1  | CREATE TABLE
   sharedserver_1  | CREATE TABLE
   sharedserver_1  | CREATE TABLE
   sharedserver_1  | [nodemon] 1.11.0
   sharedserver_1  | [nodemon] to restart at any time, enter `rs`
   sharedserver_1  | [nodemon] watching: *.*
   sharedserver_1  | [nodemon] starting `node server.js`
   sharedserver_1  | App listening on port 80

Luego de esto, tanto la API como la interfaz gráfica estarán disponibles en ``http://localhost``. Notar que se está utilizando el puerto 80. Para modificar ese puerto, se puede utilizar el archivo ``docker-compose.yml``, de acuerdo con la documentación de `docker-compose`.

Deploy a Heroku
---------------

La aplicación utiliza las variables de entorno especificadas por heroku, por lo que no es necesario modificar código ni archivos de configuración. Utilizando el proceso `estandar de deploy de heroku <https://devcenter.heroku.com/articles/getting-started-with-nodejs#introduction/>`_ puede tenerse la aplicacion en producción sin trabajo adicional.