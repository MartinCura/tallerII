package ar.fiuba.jobify.utils;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Address;
import android.location.Geocoder;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.IdRes;
import android.support.annotation.Nullable;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.widget.EditText;
import android.widget.Toast;

import java.io.IOException;
import java.util.List;
import java.util.Locale;

import ar.fiuba.jobify.PerfilActivity;

/**
 * Created by martín on 06/11/16.
 * Wrapper para clases creadas para ser usadas en PerfilActivity.
 */
public class PerfilUtils {
    /*---------- Listener class to get coordinates ------------- */
    public static class MyLocationService implements LocationListener {

        private final static String LOG_TAG = MyLocationService.class.getSimpleName();

        Context ctx;
        EditText et_city;

        LocationManager mLocationManager;

        Location mLastLocation = null;
        String mCityName = null;

        // Llamar con getBaseContext(). No olvidarse de llamar a finish().
        public MyLocationService(Activity activity, @IdRes int editTextId) {
            ctx = activity.getBaseContext();

            if ( !(Build.VERSION.SDK_INT < Build.VERSION_CODES.M)
                    && ContextCompat.checkSelfPermission(activity, android.Manifest
                    .permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED ) {

                ActivityCompat.requestPermissions( activity,
                        new String[] {  android.Manifest.permission.ACCESS_COARSE_LOCATION  },
                        PerfilActivity.REQUEST_GET_LOCATION );
                throw new SecurityException();
            }

            et_city = (EditText) activity.findViewById(editTextId);
            if (et_city == null) {
                Log.e(LOG_TAG, "No encontré EditText!");
            } else {
                et_city.clearFocus();
                et_city.setFocusable(false);
            }

            mLocationManager = (LocationManager) ctx.getSystemService(Context.LOCATION_SERVICE);
            mLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000, 100,
                    this);

            mLastLocation = mLocationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
            mCityName = obtenerCiudadParaLocation(mLastLocation);
            if (et_city != null)
                et_city.setText(mCityName);
        }

        /**
         * No olvidar llamarlo para reestablecer el orden del universo...
         * y no perder toda la batería.
         */
        public void finish() {
            try {
                mLocationManager.removeUpdates(this);
            } catch (SecurityException ex) {
                Log.w(LOG_TAG, "Security exception??");
            }
            et_city.setFocusable(true);
            et_city.setFocusableInTouchMode(true);
        }

        @Nullable
        public Location getLocation() {
            return mLastLocation;
        }

//        @Nullable
//        public String getCityName() {
//            return mCityName;
//        }

        @Override
        public void onLocationChanged(Location loc) {
            Toast.makeText(
                    ctx,
                    "Location changed: Lat: " + loc.getLatitude() + " Lng: " + loc.getLongitude(),
                    Toast.LENGTH_SHORT).show();//
//            String longitude = "Longitude: " + loc.getLongitude();
//            String latitude = "Latitude: " + loc.getLatitude();

            String cityName = obtenerCiudadParaLocation(loc);
//            String s = longitude + "\n" + latitude + "\n\nMy Current City is: " + cityName;//
//            Log.d(LOG_TAG, s);//

            if (isBetterLocation(loc, mLastLocation)) {
                mLastLocation = loc;
                if (cityName != null) {
                    mCityName = cityName;
                }
            }
            if (et_city != null)
                et_city.setText(cityName);
        }

        @Override
        public void onProviderDisabled(String provider) {}

        @Override
        public void onProviderEnabled(String provider) {}

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {}

        @Nullable
        private String obtenerCiudadParaLocation(Location loc) {
            if (loc == null)
                return null;

            Geocoder gcd = new Geocoder(ctx, Locale.getDefault());
            List<Address> addresses;
            try {
                addresses = gcd.getFromLocation(loc.getLatitude(), loc.getLongitude(), 1);
                if (addresses.size() > 0) {
                    System.out.println(addresses.get(0).getLocality());
                    return addresses.get(0).getLocality();
                }
            }
            catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }

        private static final int TWO_MINUTES = 1000 * 60 * 2;

        /** Determines whether one Location reading is better than the current Location fix
         * @param newLocation  The new Location that you want to evaluate
         * @param currentBestLocation  The current Location fix, to which you want to compare the new one
         */
        public static boolean isBetterLocation(Location newLocation, Location currentBestLocation) {
            if (currentBestLocation == null) {
                // A new location is always better than no location
                return true;
            }

            // Check whether the new location fix is newer or older
            long timeDelta = newLocation.getTime() - currentBestLocation.getTime();
            boolean isSignificantlyNewer = timeDelta > TWO_MINUTES;
            boolean isSignificantlyOlder = timeDelta < -TWO_MINUTES;
            boolean isNewer = timeDelta > 0;

            // If it's been more than two minutes since the current location, use the new location
            // because the user has likely moved
            if (isSignificantlyNewer) {
                return true;
                // If the new location is more than two minutes older, it must be worse
            } else if (isSignificantlyOlder) {
                return false;
            }

            // Check whether the new location fix is more or less accurate
            int accuracyDelta = (int) (newLocation.getAccuracy() - currentBestLocation.getAccuracy());
            boolean isLessAccurate = accuracyDelta > 0;
            boolean isMoreAccurate = accuracyDelta < 0;
            boolean isSignificantlyLessAccurate = accuracyDelta > 200;

            // Check if the old and new location are from the same provider
            boolean isFromSameProvider = isSameProvider(newLocation.getProvider(),
                    currentBestLocation.getProvider());

            // Determine location quality using a combination of timeliness and accuracy
            if (isMoreAccurate) {
                return true;
            } else if (isNewer && !isLessAccurate) {
                return true;
            } else if (isNewer && !isSignificantlyLessAccurate && isFromSameProvider) {
                return true;
            }
            return false;
        }

        /** Checks whether two providers are the same */
        private static boolean isSameProvider(String provider1, String provider2) {
            if (provider1 == null) {
                return provider2 == null;
            }
            return provider1.equals(provider2);
        }
    }

    // TODO pasar TODO
}
