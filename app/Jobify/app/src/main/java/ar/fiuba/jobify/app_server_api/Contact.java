package ar.fiuba.jobify.app_server_api;

import com.google.gson.annotations.SerializedName;

/**
 * Created by martín on 30/09/16.
 * Versión reducida de un usuario con estado de amistad, para mostrarse como relacionado a otro.
 */
public class Contact {

    long        id;
    String      firstName = "",
                lastName = "";
    Employment  currentJob;
    Status      status = Status.NONE;


    public Contact(long id, String firstName, String lastName, Employment trabajo, Status status) {
        this.id = id;
        this.firstName = firstName;
        this.lastName = lastName;
        this.currentJob = new Employment(trabajo);
        this.status = status;
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

    public Employment getCurrentJob() {
        return currentJob;
    }

    public Status getStatus() {
        return status;
    }



    public enum Status {

        @SerializedName("") // Puede causar errores
        NONE (0),

        @SerializedName("requested")
        REQUESTED (1),

        @SerializedName("received")
        RECEIVED (2),

        @SerializedName("active")
        ACTIVE (3);

        private final int value;

        Status(int value) {
            this.value = value;
        }

        @SuppressWarnings("unused")
        public int getValue() {
            return value;
        }

    }
}
