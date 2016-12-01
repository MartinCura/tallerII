Documentación ténica de funcionamiento
======================================

Tecnologías y librerías
-----------------------

Como se especifica en el enunciado, las tecnologías principales son ``node.js`` + ``Express.js`` como servidor web y ``PostgreSQL`` como base de datos relacional.

Para ingresar a la base de datos desde el servidor web se utilizó la librería ``pg``, que permite hacer consultas asincrónicas de SQL plano a la base de datos. Se eligió esta tecnología ya que las consultas a realizar son simples, en el futuro si la aplicación se complejisa, se podría migrar a una solución más sofisticada como ``massive-js``.

Estructura y archivos importantes
---------------------------------

Servidor web
!!!!!!!!!!!!

Hay 5 archivos importantes en esta aplicación:

server.js
/////////

Archivo de inicialización de node.js, donde se importan los demás de los archivos y se lanza el servidor. Cuando queremos inicializar el servidor haremos:

``$ node server.js``

urls.js
///////

El archivo donde se mapean todas las urls soportadas con *views*, tanto por la API como la intefaz gráfica.

views.js
////////

Aquí se define la respuesta a cada una de las urls, tomando en cuenta los parametros y los headers.

db.js
/////

Donde se realizan las consultas a la base de datos, solo este archivo puede acceder a la base de datos.

db.sql
//////

Aquí se especifica el esquema de datos de la base en formato SQL. 

Para aplicarlo se ejecuta.

``psql -f db.sql``

No puede crear ningún dato ni realizar una consulta en la base de datos sin antes haber ejecutado ese comando.

Interfaz web
!!!!!!!!!!!!

/public/index.html
//////////////////

Donde se define la itnerfaz gráfica básica, que luego el archivo ``core.js`` extenderá.

/publi/core.js
//////////////

Aquí se configura Angular 1.5, se realizan las consultas a la API y los controladores de la interfaz gráfica.

Variables de entorno del servidor web
-------------------------------------

``PORT``: Puerto que escuchara la interfaz web.
``DATABASE_URL``: Url en formato PostgreSQL para configurar la conexión a la base de datos.

Tests
-----

Se implementan tests de caja negra, probando los endpoints de la API utilizando `pyresttest <https://github.com/svanoort/pyresttest/>`_. 

Para ejecutarlos::

   $ cd  tests
   $ docker-compose up

De pasar correctamente no deberíamos ver mensajes en rojo.

Los archivos que definen los tests se encuentran en el archivo ``tests/integration.yaml`` y siguen el formato estandar de ``pyresttest``.