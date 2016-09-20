package ar.fiuba.jobify.app_server_api;

import com.google.gson.annotations.SerializedName;

/**
 * Created by martín on 19/09/16.
 */
public class Employment {

    // Borrar cuando se cambie el AppServer.
    @SerializedName("positionTitle")
    String positionTitle = "";
    @SerializedName("company")
    String company = "";
    @SerializedName("fromDate")
    String fromDate = "";
    @SerializedName("toDate")
    String toDate = "";


//    String  positionTitle = "",
//            company = "",
//            fromDate = "",
//            toDate = "";


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
}