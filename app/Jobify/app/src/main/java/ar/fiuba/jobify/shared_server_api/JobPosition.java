package ar.fiuba.jobify.shared_server_api;

import android.app.Activity;

/**
 * Created by martín on 06/09/16.
 * Simple estructura que contiene un posible puesto de trabajo según el SharedData.
 */
public class JobPosition implements Nombrable {

    String  name = "",
            description = "",
            category = "";


    // copy constructor
    private JobPosition(JobPosition o) {
        this.name = o.name;
        this.description = o.description;
        this.category = o.category;
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


    // Solo permite obtener un JobPosition con un title/name que ya exista en el SharedData
    public static JobPosition create(Activity activity, String jpName) {
        JobPositionsResponse jpr = SharedDataSingleton.getInstance(activity).getJobPositionsResponse();
        JobPosition found;
        if ((found = jpr.findPosition(jpName)) == null)
            throw new IllegalArgumentException();
        return new JobPosition(found);
    }
}
