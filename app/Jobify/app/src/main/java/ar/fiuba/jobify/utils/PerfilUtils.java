package ar.fiuba.jobify.utils;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.DatePickerDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.ColorStateList;
import android.location.Address;
import android.location.Geocoder;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.IdRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.DialogFragment;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.Locale;

import ar.fiuba.jobify.PerfilActivity;
import ar.fiuba.jobify.R;
import ar.fiuba.jobify.app_server_api.Contact;
import ar.fiuba.jobify.app_server_api.ContactsResponse;
import ar.fiuba.jobify.app_server_api.Employment;
import ar.fiuba.jobify.shared_server_api.JobPosition;
import ar.fiuba.jobify.shared_server_api.SharedDataSingleton;
import ar.fiuba.jobify.shared_server_api.Skill;

/**
 * Created by martín on 06/11/16.
 * Wrapper para clases creadas para ser usadas en PerfilActivity.
 */
public class PerfilUtils {

    private final static String LOG_TAG = PerfilUtils.class.getSimpleName();

    public static final int REQUEST_TAKE_PHOTO = 1;
    public static final int REQUEST_PICK_IMAGE = 2;
    public static final int REQUEST_GET_LOCATION = 3;

    private static String mSelectedJobPosition = "";
    private static String mSelectedSkill = "";


    /*---------- Listener class to get coordinates ------------- */
    public static class MyLocationService implements LocationListener {

        private final static String LOG_TAG = MyLocationService.class.getSimpleName();

        private static final int TWO_MINUTES = 1000 * 60 * 2;
        private static final int FIFTEEN_MINUTES = 1000 * 60 * 15;

        Context ctx;
        EditText et_city;

        LocationManager mLocationManager;

        Location mLastLocation = null;
        String mCityName = null;

        // No olvidarse de llamar a finish().
        public MyLocationService(Activity activity, @IdRes int editTextId) {
            ctx = activity.getBaseContext();

            if ( !(Build.VERSION.SDK_INT < Build.VERSION_CODES.M)
                    && ContextCompat.checkSelfPermission(activity, android.Manifest
                    .permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED ) {

                ActivityCompat.requestPermissions( activity,
                        new String[] {  android.Manifest.permission.ACCESS_COARSE_LOCATION  },
                        REQUEST_GET_LOCATION );
                throw new SecurityException();
            }

            Toast.makeText(activity, "Esperando ubicación...", Toast.LENGTH_SHORT)
                    .show();

            et_city = (EditText) activity.findViewById(editTextId);
            if (et_city == null) {
                Log.e(LOG_TAG, "No encontré EditText!");
            } else {
                et_city.clearFocus();
                et_city.setFocusable(false);
            }

            mLocationManager = (LocationManager) ctx.getSystemService(Context.LOCATION_SERVICE);
            mLocationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 3000, 200, this);

            mLastLocation = mLocationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
            if (mLastLocation != null && System.currentTimeMillis() - mLastLocation.getTime() < FIFTEEN_MINUTES) {
                mCityName = obtenerCiudadParaLocation(mLastLocation);
                if (et_city != null)
                    et_city.setText(mCityName);
                Toast.makeText(activity, "Actualizando ubicación...", Toast.LENGTH_SHORT)
                        .show();
            }
        }

        public void pause() {
            try {
                mLocationManager.removeUpdates(this);
            } catch (SecurityException ex) {
                Log.w(LOG_TAG, "Security exception??");
            }
        }

        public void unpause() {
            try {
                mLocationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 3000, 200,
                        this);
            } catch (SecurityException ex) {
                Log.w(LOG_TAG, "Security exception??");
            }
        }

        /**
         * No olvidar llamarlo para reestablecer el orden del universo
         * ...y no perder toda la batería.
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
            Toast.makeText(ctx, "Actualizando ubicación...", Toast.LENGTH_SHORT)
                    .show();

            String cityName = obtenerCiudadParaLocation(loc);

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


    public static void populateSpinners(final Activity act) {
        try {
            Spinner spinner = (Spinner) act.findViewById(R.id.perfil_employment_new_position_spinner);
            if (spinner == null) {
                Log.e(LOG_TAG, "Spinner de job positions no encontrado!");
                return;
            }
            List<JobPosition> jobPositions = SharedDataSingleton.getInstance(act).getJobPositions();
            if (jobPositions != null) {

                ArrayList<String> jpArray = new ArrayList<>();
                jpArray.add(act.getString(R.string.perfil_new_job_position_filler)); // Opción vacía
                for (JobPosition jp : jobPositions) {
                    if (jp != null)
                        jpArray.add(jp.getNombre());
                }
                ArrayAdapter<String> jpAdapter =
                        new ArrayAdapter<>(act, android.R.layout.simple_spinner_item, jpArray);
                jpAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                spinner.setAdapter(jpAdapter);

                spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                    @Override
                    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
                        mSelectedJobPosition = (String) parent.getItemAtPosition(pos);
                        if (mSelectedJobPosition.equals(act.getString(R.string.perfil_new_job_position_filler)))
                            mSelectedJobPosition = "";
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parent) {
                        mSelectedJobPosition = "";
                    }
                });
            }
        } catch (SharedDataSingleton.NoDataException ex) {
//            Toast.makeText(act, "Problemas con SS.JobPositions", Toast.LENGTH_LONG)
//                    .show();//
            Log.e(LOG_TAG, "Problemas con SS.JobPositions");
        }

        try {
            Spinner spinner = (Spinner) act.findViewById(R.id.perfil_skills_new_spinner);
            if (spinner == null) {
                Log.e(LOG_TAG, "Spinner de skills no encontrado!");
                return;
            }
            List<Skill> skills = SharedDataSingleton.getInstance(act).getSkills();
            if (skills != null) {

                ArrayList<String> skArray = new ArrayList<>();
                skArray.add(act.getString(R.string.perfil_new_skill_filler)); // Opción vacía
                for (Skill sk : skills) {
                    if (sk != null)
                        skArray.add(sk.getNombre());
                }
                ArrayAdapter<String> skAdapter =
                        new ArrayAdapter<>(act, android.R.layout.simple_spinner_item, skArray);
                skAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                spinner.setAdapter(skAdapter);

                spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                    @Override
                    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
                        mSelectedSkill = (String) parent.getItemAtPosition(pos);
                        if (mSelectedSkill.equals(act.getString(R.string.perfil_new_skill_filler)))
                            mSelectedSkill = "";
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parent) {
                        mSelectedSkill = "";
                    }
                });
            }
        } catch (SharedDataSingleton.NoDataException ex) {
//            Toast.makeText(act, "Problemas con SS.Skills", Toast.LENGTH_LONG)
//                    .show();//
            Log.e(LOG_TAG, "Problemas con SS.Skills");
        }
    }
    

    public static boolean agregarEmployment(PerfilActivity act, EditableListAdapter<Employment> mJobsAdapter) {
        EditText et_company = (EditText) act.findViewById(R.id.text_perfil_employment_new_company);
        Spinner sp_position = (Spinner) act.findViewById(R.id.perfil_employment_new_position_spinner);
        EditText et_desde_mes = (EditText) act.findViewById(R.id.perfil_employment_desde_mes);
        EditText et_desde_anio = (EditText) act.findViewById(R.id.perfil_employment_desde_anio);
        EditText et_hasta_mes = (EditText) act.findViewById(R.id.perfil_employment_hasta_mes);
        EditText et_hasta_anio = (EditText) act.findViewById(R.id.perfil_employment_hasta_anio);
        // Cuidado si cambio por DatePickerDialog

        if (et_company == null || mSelectedJobPosition == null || et_desde_mes == null
                || et_desde_anio == null || et_hasta_mes == null || et_hasta_anio == null) {
            Log.e(LOG_TAG, "ET go null");
            Toast.makeText(act, "Ha ocurrido un misterioso error nulo", Toast.LENGTH_LONG)
                    .show();
            return false;
        }
        et_company.setError(null);

        String errorCampoVacio = "Este campo no puede quedar vacío";
        if (mSelectedJobPosition != null && mSelectedJobPosition.length() == 0) {
            Toast.makeText(act, "Debe seleccionar una posición de trabajo", Toast.LENGTH_LONG)
                    .show();
            return false;
        } else if (et_company.length() == 0) {
            et_company.setError(errorCampoVacio);
            et_company.requestFocus();
            return false;
        } else if (et_desde_mes.length() == 0) {
            et_desde_mes.setError(errorCampoVacio);
            et_desde_mes.requestFocus();
            return false;
        } else if (et_desde_anio.length() == 0) {
            et_desde_anio.setError(errorCampoVacio);
            et_desde_anio.requestFocus();
            return false;
        } else if ((et_hasta_mes.length() == 0 && et_hasta_anio.length() != 0)
                 || et_hasta_mes.length() != 0 && et_hasta_anio.length() == 0) {
            et_hasta_mes.setError("Para un trabajo actual, deje mes y año de 'hasta' vacíos");
            et_hasta_mes.requestFocus();
            return false;
        }

        // "Hasta" puede estar vacío
        int hastaMes = (et_hasta_mes.length() == 0) ? 0 : Integer.valueOf(et_hasta_mes.getText().toString());
        int hastaAnio = (et_hasta_anio.length() == 0) ? 0 : Integer.valueOf(et_hasta_anio.getText().toString());

        if (et_hasta_anio.length() != 0) {
            int dm = Integer.valueOf(et_desde_mes.getText().toString());
            int da = Integer.valueOf(et_desde_anio.getText().toString());
            if (da > hastaAnio || (da == hastaAnio && dm > hastaMes)) {
                et_hasta_anio.setError("Fecha 'hasta' no puede ser posterior a 'desde'");
                et_hasta_anio.requestFocus();
                return false;
            }
        }

        try {
            Employment nuevoEmployment = Employment.create(act,
                    et_company.getText().toString(),
                    mSelectedJobPosition,
                    Integer.valueOf(et_desde_mes.getText().toString()),
                    Integer.valueOf(et_desde_anio.getText().toString()),
                    hastaMes, hastaAnio);
            if (nuevoEmployment == null) return false;
            if (!mJobsAdapter.add(nuevoEmployment, true)) {
                Toast.makeText(act, "Empleo ya listado", Toast.LENGTH_LONG)
                        .show();
                return false;
            }
            et_company.setText(null);
            if (sp_position != null)
                sp_position.setSelection(0);
            et_desde_mes.setText(null);
            et_desde_anio.setText(null);
            et_hasta_mes.setText(null);
            et_hasta_anio.setText(null);
            mJobsAdapter.notifyDataSetChanged();

        } catch (IllegalArgumentException ex) {
            EditText et_error = et_company;
            switch (ex.getMessage()) {
                case "compañía":
                    et_error = et_company;
                    break;
                case "posición":
                    et_error = null;
                    break;
                case "mes desde":
                    et_error = et_desde_mes;
                    break;
                case "mes hasta":
                    et_error = et_hasta_mes;
                    break;
                case "año desde":
                    et_error = et_desde_anio;
                    break;
                case "año hasta":
                    et_error = et_hasta_anio;
                    break;
            }
            String errorStr = "Valor inválido para " + ex.getMessage();
            if (et_error != null) {
                et_error.setError(errorStr);
                et_error.requestFocus();
            } else {
                Toast.makeText(act, errorStr, Toast.LENGTH_LONG)
                        .show();
            }
            return false;
        }
        return true;
    }

    public static boolean agregarSkill(PerfilActivity act, EditableListAdapter<Skill> mSkillAdapter) {
        Spinner sp_skill = (Spinner) act.findViewById(R.id.perfil_skills_new_spinner);
        if (mSelectedSkill == null)
            return false;
        else if (mSelectedSkill.length() == 0) {
            Toast.makeText(act, "Debe seleccionar un skill", Toast.LENGTH_LONG)
                    .show();
        }

        try {
            Skill nuevoSkill = Skill.create(act, mSelectedSkill);
            if (nuevoSkill == null) return false;
            if (!mSkillAdapter.add(nuevoSkill, true)) {
                Toast.makeText(act, "Skill ya listado", Toast.LENGTH_LONG)
                        .show();
                return false;
            }
            if (sp_skill != null)
                sp_skill.setSelection(0);
            mSkillAdapter.notifyDataSetChanged();

        } catch (IllegalArgumentException ex) {
            Toast.makeText(act, "Skill desconocido", Toast.LENGTH_LONG)
                    .show();
            return false;
        }

        return true;
    }


    public static void colorearBotonAmistad(PerfilActivity act, Contact.Status estado) {

        FloatingActionButton fab_amigar = (FloatingActionButton) act.findViewById(R.id.fab_amigar);
        if (fab_amigar == null) {
            Log.e(LOG_TAG, "No pude encontrar fab_amigar");
            return;
        }
        ColorStateList csl;

        switch (estado) {
            case REQUESTED:
                csl = Utils.getColorStateList(act, R.color.amigar_btn_requested);
                break;
            case RECEIVED:
                csl = Utils.getColorStateList(act, R.color.amigar_btn_received);
                break;
            case ACTIVE:
                csl = Utils.getColorStateList(act, R.color.amigar_btn_active);
                break;
            case NONE:
                csl = Utils.getColorStateList(act, R.color.amigar_btn_none);
                break;
            default:
                csl = Utils.getColorStateList(act, R.color.amigar_btn_none);
                fab_amigar.setEnabled(false);
        }
        fab_amigar.setBackgroundTintList(csl);
        fab_amigar.clearAnimation();
        fab_amigar.refreshDrawableState();
        fab_amigar.setPressed(true);
        fab_amigar.setPressed(false);
        fab_amigar.clearFocus();
    }

    public static void colorearBotonRecomendar(PerfilActivity act, boolean recomendado) {

        FloatingActionButton fab_recomendar = (FloatingActionButton) act.findViewById(R.id.fab_recomendar);
        if (fab_recomendar == null) {
            Log.e(LOG_TAG, "No pude encontrar fab_recomendar");
            return;
        }

        ColorStateList csl;
        if (recomendado) {
            csl = Utils.getColorStateList(act, R.color.recomendar_btn_true);
        } else {
            csl = Utils.getColorStateList(act, R.color.recomendar_btn_false);
        }
        fab_recomendar.setBackgroundTintList(csl);
        fab_recomendar.clearAnimation();
        fab_recomendar.refreshDrawableState();
        fab_recomendar.setPressed(false);
        fab_recomendar.clearFocus();
    }


    public static void dispatchChoosePictureIntent(PerfilActivity act) {
        Intent getIntent = new Intent(Intent.ACTION_GET_CONTENT);
        getIntent.setType("image/*");

        Intent pickIntent = new Intent(Intent.ACTION_PICK,
                android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        pickIntent.setType("image/*");

        Intent chooserIntent = Intent.createChooser(getIntent, "Select Image");
        chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, new Intent[] {pickIntent});

        if (chooserIntent.resolveActivity(act.getPackageManager()) != null) {
            act.startActivityForResult(chooserIntent, REQUEST_PICK_IMAGE);
        }
    }


    public static void populateContacts(final PerfilActivity act, ContactsResponse response,
                                        long connectedUserID) {

        // Obtengo el estado de amistad del usuario fetched con el connected para colorear el botón
        Contact.Status estado = response.getStatusForId(connectedUserID);
        PerfilUtils.colorearBotonAmistad(act, estado);

        ArrayList<Contact> activeContacts = response.getContactsWithStatus(Contact.Status.ACTIVE);

        if (activeContacts.size() == 0) {
            Utils.hideView(act, R.id.perfil_contactos_frame);
            return;
        } else {
            Utils.showView(act, R.id.perfil_contactos_frame);
        }

        HorizontalListView mHLView = (HorizontalListView) act.findViewById(R.id.perfil_contactos_list);
        if (mHLView != null) {

            final ContactCardAdapter mContactCardAdapter =
                    new ContactCardAdapter(act, mHLView, activeContacts);
            mHLView.setAdapter(mContactCardAdapter);
            mHLView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    Contact clickedUser = (Contact) mContactCardAdapter.getItem(position);
                    act.startActivity(
                            new Intent(PerfilActivity.getContext(), PerfilActivity.class)
                                    .putExtra(PerfilActivity.FETCHED_USER_ID_MESSAGE, clickedUser.getId())
                    );
                }
            });

            TextView tv_cantContactos = (TextView) act.findViewById(R.id.text_perfil_cant_contactos);
            if (tv_cantContactos != null) {
                tv_cantContactos.setText(String.valueOf(mContactCardAdapter.getCount()));
            }

        } else {
            Log.e(LOG_TAG, "No se encontró el gridview de contactos!");
        }
    }

    private static class ContactCardAdapter extends BaseAdapter {

        private final static String LOG_TAG = ContactCardAdapter.class.getSimpleName();

        PerfilActivity act;
        ArrayList<Contact> mContacts;
        HorizontalListView mHLView;
        int highestHeight = 0;

        public ContactCardAdapter(PerfilActivity perfilActivity, HorizontalListView hlv,
                                  List<Contact> contactList) {
            this.act = perfilActivity;
            this.mHLView = hlv;
            this.mContacts = new ArrayList<>(contactList);
        }

        @Override
        public long getItemId(int position) {
            return mContacts.get(position).getId();
        }

        @Override
        public Object getItem(int position) {
            return mContacts.get(position);
        }

        @Override
        public int getCount() {
            return mContacts.size();
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            View itemView = convertView;
            if (itemView == null) {
                itemView = LayoutInflater.from(PerfilActivity.getContext())
                        .inflate(R.layout.contact_card, parent, false);
            }

            Contact contact = (Contact) getItem(position);
            if (contact != null) {

                String photoUrl = Utils.getAppServerProfileUrl(PerfilActivity.getContext(), contact.getId());
                if (!Utils.cargarImagenDeURLenImageView(act.getApplicationContext(),
                        (ImageView) itemView.findViewById(R.id.contact_card_foto),
                        photoUrl, LOG_TAG)) {
                    Log.d(LOG_TAG, "No encontré ImageView para contacto en Perfil");
                }

                TextView tv_nombre  = (TextView) itemView.findViewById(R.id.contact_card_nombre);
                if (tv_nombre != null)
                    tv_nombre.setText(contact.getFullName());

                TextView tv_trabajo = (TextView) itemView.findViewById(R.id.contact_card_trabajo);
                if (tv_trabajo != null) {
                    Employment currJob = contact.getCurrentJob();
                    if (currJob != null)
                        tv_trabajo.setText(currJob.getOneLiner());
                }

                // Corrijo altura de la lista horizontal de contactos
                itemView.measure(0, 0);
                int altura = itemView.getMeasuredHeight();
                if (altura > highestHeight) {
                    highestHeight = altura;
                    actualizarAlturaDeView();
                }
            }

            return itemView;
        }

        private void actualizarAlturaDeView() {
            ViewGroup.LayoutParams params = mHLView.getLayoutParams();
            params.height = this.highestHeight;
            mHLView.setLayoutParams(params);
        }
    }


    public static class DatePickerFragment extends DialogFragment
            implements DatePickerDialog.OnDateSetListener {

        PerfilActivity activity;

        @Override @NonNull
        public Dialog onCreateDialog(Bundle savedInstanceState) {
            // No es lo más lindo del mundo...
            // Refactor: crear interfaz que implementan, etc.
            activity = (PerfilActivity) getActivity();

            // Use the current date pero en 1990 as the default date in the picker
            final Calendar c = Calendar.getInstance();
            int year = 1990;
            int month = c.get(Calendar.MONTH);
            int day = c.get(Calendar.DAY_OF_MONTH);

            // Create a new instance of DatePickerDialog and return it
            return new DatePickerDialog(getActivity(), this, year, month, day);
        }

        public void onDateSet(DatePicker view, int year, int month, int day) {
            int mesBien = month + 1;
            Utils.setTextViewText(activity, R.id.perfil_nacimiento_dia, Integer.toString(day));
            Utils.setTextViewText(activity, R.id.perfil_nacimiento_mes, Integer.toString(mesBien));
            Utils.setTextViewText(activity, R.id.perfil_nacimiento_anio, Integer.toString(year));
        }
    }

    /**
     * Shows the progress UI and hides the perfil info.
     */
    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    public static void showProgress(PerfilActivity act, final boolean show) {
        final LinearLayout linearLayout = (LinearLayout) act.findViewById(R.id.perfil_information_layout);
        final View progressView = act.findViewById(R.id.perfil_progress);
        if (linearLayout == null || progressView == null) {
            Log.e(LOG_TAG, "No pude encontrar el layout de info o el progress loader.");
            return;
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
            int shortAnimTime = act.getResources().getInteger(android.R.integer.config_shortAnimTime);

            linearLayout.setVisibility(show ? View.GONE : View.VISIBLE);
            linearLayout.animate().setDuration(shortAnimTime).alpha(
                    show ? 0 : 1).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    linearLayout.setVisibility(show ? View.GONE : View.VISIBLE);
                }
            });

            progressView.setVisibility(show ? View.VISIBLE : View.GONE);
            progressView.animate().setDuration(shortAnimTime).alpha(
                    show ? 1 : 0).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    progressView.setVisibility(show ? View.VISIBLE : View.GONE);
                }
            });
        } else {
            // The ViewPropertyAnimator APIs are not available, so simply show
            // and hide the relevant UI components.
            progressView.setVisibility(show ? View.VISIBLE : View.GONE);
            linearLayout.setVisibility(show ? View.GONE : View.VISIBLE);
        }
    }
}
