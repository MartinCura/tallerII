package ar.fiuba.jobify.shared_server_api;

/**
 * Created by martín on 06/09/16.
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
