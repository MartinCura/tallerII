package ar.fiuba.jobify.shared_server_api;

import com.google.gson.annotations.SerializedName;

/**
 * Created by martín on 06/09/16.
 * Estructura API para el metadata en un response.
 */
public class ResponseMetadata {

    String  version = "";
    @SerializedName(value="count", alternate={"total_unread_count"})
    long      count = 0;
    long totalCount = 0;


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
