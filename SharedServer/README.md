Código y test para el Shared  Server
------------------------------------

obtener un ambiente con docker


`sudo docker run -it -p80:80  -vcode/:/home node bash`

create databases:

`psql -h db -U postgres -W postgres -f db.sql`

