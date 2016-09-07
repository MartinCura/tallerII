package ar.fiuba.jobify.shared_server_api;

/**
 * Created by root on 06/09/16.
 */
public class JobPosition {

    String
            name,
            description,    // TODO: Debería inicializarlas?
            category;


    public String getName() {
        return name;
    }

    public String getDescription() {
        return description;
    }

    public String getCategory() {
        return category;
    }
}
