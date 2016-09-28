package ar.fiuba.jobify.app_server_api;

import com.google.gson.annotations.SerializedName;

import ar.fiuba.jobify.shared_server_api.Nombrable;

/**
 * Created by martín on 19/09/16.
 * Simple estructura que contiene un trabajo pasado. TODO: Crear únicamente a través de Fábrica.
 */
public class Employment implements Nombrable {

    String  positionTitle = "",
            company = "",
            fromDate = "",
            toDate = "";


    public Employment(String nombre) {
        this.positionTitle = nombre;// Constructor de prueba nomás, depender de Fábrica
    }

    public String getPositionTitle() {
        return positionTitle;
    }

    public String getCompany() {
        return company;
    }

    public String getFromDate() {
        return fromDate;
    }

    public String getToDate() {
        return toDate;
    }

    public boolean esActual() {
        return (getToDate().equals(""));
    }

    /**
     * Devuelve un String del formato {@code [jobPosition] en [company]}.
      */
    public String getOneLiner() {
        return getPositionTitle() + " en " + getCompany();
    }
    public String getNombre() {
        return getOneLiner();
    }
}
