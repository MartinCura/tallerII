package ar.fiuba.jobify.app_server_api;

/**
 * Created by martín on 30/09/16.
 * Versión reducida de un usuario, para mostrarse como relacionado a otro.
 */
public class Contact {

    // TODO: Lo prolijo sería hacerlo con ints (y un serializer para Gson)
    public static final String ACTIVE = "active",
                            REQUESTED = "requested",
                            RECEIVED = "received";

    public static final String[] ContactEstados = { ACTIVE, REQUESTED, RECEIVED };

    long    id;
    String  firstName = "",
            lastName = "";
    Employment trabajoActual;
    String  estado = "";


    public Contact(long id, String firstName, String lastName, Employment trabajo, String estado) {
        this.id = id;
        this.firstName = firstName;
        this.lastName = lastName;
        this.trabajoActual = new Employment(trabajo);
        this.estado = estado;
    }


    public long getId() {
        return id;
    }

    public String getFirstName() {
        return firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public String getFullName() {
        return getFirstName() + " " + getLastName();
    }

    public Employment getTrabajoActual() {
        return trabajoActual;
    }

    public String getEstado() {
        return estado;
    }
}
