# ApplicationServer

**Instalación y ejecución de partir de un paquete Debian**

En la carpeta donde esté ubicado el paquete Debian, correr el comando:
```
dpkg -i ApplicationServer-0.1.1-Linux.deb
```
Esto instalará la aplicación en /usr/appServer. Para iniciar el servidor, correr el ejecutable ubicado en dicha carpeta. La aplicación utilizará el puerto 8000.

**Ejecución del servidor desde un contenedor de Docker**

Para iniciar el servidor dentro de un contenedor de Docker, que luego será accesible en el puerto 8000, ejecutar:
```
/tallerII$ docker build -t appserverimg .
/tallerII$ docker run --name appserver -p 8000:8000 appserverimg
```
Para detener la ejecución del contenedor, y por lo tanto del servidor, ejecutar: 
```
docker stop appserver
```

**Instalación y ejecución manual**

Ejecutar el siguiente comando:
```
/tallerII$ sudo ./INSTALL.sh
```
Esto creará un archivo ejecutable dentro de la carpeta build. Correr dicho ejecutable para iniciar el servidor, el cual utilizará el puerto 8000.

**Paquete Debian**

Para generar un paquete Debian es necesario ejecutar:
```
/tallerII$ sudo ./BUILD_DEB_PACK.sh
```
Esto creará un paquete Debian dentro de la carpeta build.

**Servicio de Log**

La aplicación cuenta con un servicio de log, que registra los acontecimientos más importantes durante la ejecución. Los archivos de log se almacenan por defecto en la carpeta “/usr/appServer/logs”. Se genera un archivo de log por mes, cuyo nombre es “YYYY-mm.log”. Por ejemplo para el mes de septiembre el nombre del archivo de log es: “2016-09.log”.

**Pruebas Unitarias**

Para correr las pruebas unitarias, ejecutar el siguiente comando:
```
/tallerII$ sudo ./RUN_TESTS.sh
```

**Coverage**

Para generar un reporte de coverage, ejecutar el siguiente comando:
```
/tallerII$ sudo ./BUILD_COVERAGE_REPORT.sh
```
Esto creará un archivo llamado “index.html” en la carpeta tallerII/build/coverage. Al abrir dicho archivo con cualquier browser se puede observar el reporte del coverage.

**Configuración**

En el directorio /usr/appServer se encuentra el archivo “config.js”. Este archivo contiene las siguientes configuraciones:
```
{
    "DbName" : <Ruta completa al directorio donde se almacena la base de datos>,
    "imgDir": <Ruta completa al directorio donde se almacenan las imágenes>,
    "logsDir": <Ruta completa al directorio donde se almacenan los archivos de log>,
    "facebookUrl": <Url de Facebook para obtener datos de usuarios>,
    "firebaseUrl": <Url de Firebase para enviar notificaciones>",
    "firebaseAuthorizationKey": <Token de autorización para Firebase>,
    "dummyServerEnabled":  <true/false. Cuando su valor es “true” deshabilita las conexiones con otros servidores. Para propósitos de prueba.>
}
```

