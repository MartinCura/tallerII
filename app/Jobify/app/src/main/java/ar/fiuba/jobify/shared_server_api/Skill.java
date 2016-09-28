package ar.fiuba.jobify.shared_server_api;

/**
 * Created by martín on 06/09/16.
 * Simple estructura que contiene un skill. TODO: Crear únicamente a través de Fábrica.
 */
public class Skill implements Nombrable {

    String
            name = "",
            description = "",
            category = "";


    public Skill(String nombre) {
        this.name = nombre;// Constructor de prueba nomás, depender de Fábrica
    }

    public String getName() {
        return name;
    }
    public String getNombre() {
        return getName();
    }

    public String getDescription() {
        return description;
    }

    public String getCategory() {
        return category;
    }
}
