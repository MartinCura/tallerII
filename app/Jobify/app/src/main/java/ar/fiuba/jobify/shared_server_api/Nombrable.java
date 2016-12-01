package ar.fiuba.jobify.shared_server_api;

/**
 * Created by martín on 27/09/16.
 * Para ser listada.
 */
public interface Nombrable {

    String getNombre();

    @Override
    boolean equals(Object o);
}
