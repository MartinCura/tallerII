package ar.fiuba.jobify.app_server_api;

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
        return (setLatitude(latitud) && setLongitude(longitud));
    }

    public boolean setLatitude(double latitude) {
        if (latitude < -90 || latitude > 90)
            return false;
        this.latitude = latitude;
        return true;
    }

    public boolean setLongitude(double longitude) {
        if (longitude < -180 || longitude > 180)
            return false;
        this.longitude = longitude;
        return true;
    }
}
