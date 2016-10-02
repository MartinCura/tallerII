package ar.fiuba.jobify.app_server_api;

/**
 * Created by martín on 01/10/16.
 * Estructura de una solicitud de contacto como debe ser enviada al AppServer.
 */
public class Solicitud {

    long id = 0;
    Contact.Status status;

    public Solicitud(long id, Contact.Status status) {
        this.id = id;
        this.status = status;
    }

    public long getId() {
        return id;
    }

    public Contact.Status getStatus() {
        return status;
    }
}
