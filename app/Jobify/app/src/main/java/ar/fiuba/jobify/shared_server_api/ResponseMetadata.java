package ar.fiuba.jobify.shared_server_api;

/**
 * Created by martín on 06/09/16.
 * Estructura API para el metadata en un response.
 */
public class ResponseMetadata {

    String version;
    int count;


    public String getVersion() {
        return version;
    }

    public int getCount() {
        return count;
    }
}
