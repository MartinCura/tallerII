package ar.fiuba.jobify.app_server_api;

import android.location.Location;
import android.support.annotation.Nullable;
import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

/**
 * Created by martín on 06/11/16.
 * Estructura API simple que contiene coordenadas de un User.
 */
public class Locacion {

    double  latitude  = 0,
            longitude = 0;

    public double getLatitude() {
        return latitude;
    }

    public double getLongitude() {
        return longitude;
    }

    public boolean setLocation(double latitud, double longitud) {
        return validLongitude(longitud)
            && setLatitude(latitud) && setLongitude(longitud);
    }

    public boolean setLatitude(double latitude) {
        if (!validLatitude(latitude))
            return false;
        this.latitude = latitude;
        return true;
    }

    public boolean setLongitude(double longitude) {
        if (!validLongitude(longitude))
            return false;
        this.longitude = longitude;
        return true;
    }

    private static boolean validLatitude(double latitude) {
        return !(latitude < -90 || latitude > 90);
    }

    private static boolean validLongitude(double longitude) {
        return !(longitude < -180 || longitude > 180);
    }


    public Locacion() {}

    public Locacion(Location location) {
        this.latitude = location.getLatitude();
        this.longitude = location.getLongitude();
    }

    public String toJson() {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();
        return gson.toJson(this);
    }

    @Nullable
    public static Locacion parseJson(String str) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();
        try {
            return gson.fromJson(str, Locacion.class);
        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }
}
