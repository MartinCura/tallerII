package ar.fiuba.jobify.app_server_api;

import android.app.Activity;
import android.support.annotation.Nullable;

import java.util.Calendar;

import ar.fiuba.jobify.shared_server_api.JobPositionsResponse;
import ar.fiuba.jobify.shared_server_api.Nombrable;
import ar.fiuba.jobify.shared_server_api.SharedDataSingleton;

/**
 * Created by martín on 19/09/16.
 * Simple estructura que contiene un trabajo pasado.
 */
public class Employment implements Nombrable {

    String  positionTitle = "",
            company = "",
            fromDate = "",
            toDate = "";

    private Employment(String company, String position, String desde, String hasta) {
        this.company = company;
        this.positionTitle = position;
        this.fromDate = desde;
        this.toDate = hasta;
    }

    // copy constructor
    public Employment(Employment o) {
        this.positionTitle = o.positionTitle;
        this.company = o.company;
        this.fromDate = o.fromDate;
        this.toDate = o.toDate;
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
        if (toDate == null) return "";
        return toDate;
    }

    public boolean esActual() {
        return (getToDate() == null || getToDate().equals(""));
    }

    /**
     * Devuelve un String del formato {@code [jobPosition] en [company]}.
      */
    public String getOneLiner() {
        return getPositionTitle() + " en " + getCompany();
    }

    public String getCompleto() {
        String ret = getPositionTitle() + " en " + getCompany() + "\n"
                + "desde " + getFromDate();
        if (!getToDate().isEmpty())
            ret += " hasta " +getToDate();
        return ret;
    }

    // Pedido por la interfaz Nombrable para obtener un String que lo represente en una listaZ
    public String getNombre() {
        return getCompleto();
    }

    // Igualdad se determina por igual puesto, compañía, e inicio o fin
    @Override
    public boolean equals(Object o) {
        return ((o instanceof Employment)
                && (this.getCompany().equals(((Employment) o).getCompany()))
                && (this.getPositionTitle().equals(((Employment) o).getPositionTitle()))
                && ((this.getFromDate().equals(((Employment) o).getFromDate()))
                    || this.getToDate().equals(((Employment) o).getToDate())));
    }

    // Permite obtener un Employment con un JobPosition que ya exista en el SharedData
    @Nullable
    public static Employment create(Activity activity, String company, String position,
                                    int desdeMes, int desdeAnio, int hastaMes, int hastaAnio) {
        String sdPosition;
        JobPositionsResponse jpr = SharedDataSingleton.getInstance(activity).getJobPositionsResponse();
        if (jpr == null) return null;

        if (company.isEmpty())
            throw new IllegalArgumentException("compañía");
        if (position.isEmpty() || (sdPosition = jpr.findPositionTitle(position)) == null)
            throw new IllegalArgumentException("posición");
        if (desdeMes <= 0 || desdeMes > 12)
            throw new IllegalArgumentException("mes desde");
        if (hastaMes  < 0 || hastaMes > 12)
            throw new IllegalArgumentException("mes hasta");
        int currentYear = Calendar.getInstance().get(Calendar.YEAR);
        if (desdeAnio < 1900 || desdeAnio > currentYear)
            throw new IllegalArgumentException("año desde");
        if ((hastaAnio < 1900 && hastaAnio != 0) || hastaAnio > currentYear)
            throw new IllegalArgumentException("año hasta");

        String desde = ((desdeMes < 10) ? "0"+desdeMes : ""+desdeMes) + "/" + desdeAnio;
        String hasta = "";
        if (hastaMes != 0 && hastaAnio != 0)
            hasta = ((hastaMes < 10) ? "0"+hastaMes : ""+hastaMes) + "/" + hastaAnio;

        return new Employment(company, sdPosition, desde, hasta);
    }
}
