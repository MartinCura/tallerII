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
}
