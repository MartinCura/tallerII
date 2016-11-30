package ar.fiuba.jobify.shared_server_api;

/**
 * Created by martín on 06/09/16.
 * Simple estructura que contiene un posible puesto de trabajo según el SharedData.
 */
public class JobPosition implements Nombrable {

    String
            name = "",
            description = "",
            category = "";


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
