package ar.fiuba.jobify.shared_server_api;

/**
 * Created by martín on 06/09/16.
 * Estructura API para el metadata en un response.
 */
public class ResponseMetadata {

    String  version;
    long     count = 0,
        totalCount = 0;


    public String getVersion() {
        return version;
    }

    public long getCount() {
        return count;
    }

    public long getTotalCount() {
        return totalCount;
    }
}
