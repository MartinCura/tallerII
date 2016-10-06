package ar.fiuba.jobify;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.ColorStateList;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.IdRes;
import android.support.annotation.LayoutRes;
import android.support.design.widget.CollapsingToolbarLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.content.FileProvider;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.BaseAdapter;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;

import org.json.JSONObject;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.app_server_api.Contact;
import ar.fiuba.jobify.app_server_api.ContactsResponse;
import ar.fiuba.jobify.app_server_api.Employment;
import ar.fiuba.jobify.app_server_api.User;
import ar.fiuba.jobify.shared_server_api.JobPosition;
import ar.fiuba.jobify.shared_server_api.SharedDataSingleton;
import ar.fiuba.jobify.shared_server_api.Skill;

public class PerfilActivity extends NavDrawerActivity {

    private final String LOG_TAG = PerfilActivity.class.getSimpleName();

    public final static String FETCHED_USER_ID_MESSAGE = "ar.fiuba.jobify.FETCHED_USER_ID_MESSAGE";
    public final static String MODO_PERFIL_MESSAGE = "ar.fiuba.jobify.MODO_PERFIL_MESSAGE";
    private long fetchedUserID = 2;
    private User fetchedUser;

    static final int REQUEST_TAKE_PHOTO = 1;
    static final int REQUEST_PICK_IMAGE = 2;
    String mCurrentPhotoPath = null;
    Uri mPhotoURI;

    private static Context mContext;

    private CollapsingToolbarLayout collapsingToolbarLayout;//
    private boolean inEditingMode = false;  // TODO: revisar qué ocurre si giro la pantalla
    private EditableListAdapter<Skill> mSkillAdapter;
    private EditableListAdapter<Employment> mJobsAdapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mContext = this;
        setContentView(R.layout.activity_perfil_drawer);

        // Obtengo el id del usuario que debo mostrar
        Intent intent = getIntent();
        if (intent != null && intent.hasExtra(FETCHED_USER_ID_MESSAGE)) {
            fetchedUserID = intent.getLongExtra(FETCHED_USER_ID_MESSAGE, fetchedUserID);
        }

        collapsingToolbarLayout =
                (CollapsingToolbarLayout) findViewById(R.id.collapsing_toolbar_perfil);

        FloatingActionButton fabAmigar = (FloatingActionButton) findViewById(R.id.fab_amigar);
        if (fabAmigar != null) {
            fabAmigar.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (fetchedUser != null && fetchedUser.getId() != connectedUserID)
                        modificarAmistad();
                    // TODO: Amigar
                }
            });
        }

        FloatingActionButton fabRecomendar = (FloatingActionButton) findViewById(R.id.fab_recomendar);
        if (fabRecomendar != null) {
            fabRecomendar.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    // TODO: Recomendar
                }
            });
        }

        FloatingActionButton fabChatear = (FloatingActionButton) findViewById(R.id.fab_chatear);
        if (fabChatear != null) {
            fabChatear.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    // TODO: Chatear
                }
            });
        }

        FloatingActionButton fabEditar = (FloatingActionButton) findViewById(R.id.fab_editar);
        if (fabEditar != null) {
            fabEditar.setBackgroundTintList(Utils.getColorStateList(this, R.color.editar_btn_inactive));
            fabEditar.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (fetchedUser != null)
                        toggleEditMode();
                }
            });
        }

        populateAutoCompleteLists();

        if (fetchedUserID == connectedUserID) {
            if (fabAmigar != null) fabAmigar.setVisibility(View.GONE);
            if (fabRecomendar != null) fabRecomendar.setVisibility(View.GONE);
            if (fabChatear != null) fabChatear.setVisibility(View.GONE);
            if (fabEditar != null) fabEditar.setVisibility(View.VISIBLE);
        }

        Utils.toggleViewVisibility(this, R.id.perfil_information_layout);
    }

    @Override
    public void setContentView(@LayoutRes int layoutResID) {
        super.setContentView(layoutResID);
        onCreateDrawer(R.id.perfil_toolbar, R.id.perfil_drawer_layout, R.id.perfil_nav_view);
    }

    @Override
    protected void onResume() {
        super.onResume();

        refreshProfileInformation(fetchedUserID);
        cargarFotoDePerfil(fetchedUserID);

        // TODO: REVISAR, YA QUE ESTÁ EN EL onResume
        // Obtengo el modo en el que debe comenzar
        Intent intent = getIntent();
        if (intent != null && intent.hasExtra(MODO_PERFIL_MESSAGE)) {

            boolean empezarEnModoEdicion = intent.getBooleanExtra(MODO_PERFIL_MESSAGE, false);
            if (empezarEnModoEdicion) {

                Utils.showView(this, R.id.perfil_information_layout);

                inEditingMode = false;
                toggleEditMode();
            }
        }
    }

    @Override
    protected void onStop() {
        super.onStop();

        if (RequestQueueSingleton.hasRequestQueue()) {  // TODO: Llamar a esto acá? Revisar.

            RequestQueue mRequestQueue = RequestQueueSingleton
                    .getInstance(this.getApplicationContext())
                    .getRequestQueue();
            mRequestQueue.cancelAll(LOG_TAG);
        }
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(mDrawerResId);
        if (drawer != null && drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else if (inEditingMode) {
            toggleEditMode();
        } else {
            super.onBackPressed();
        }
    }

    public static Context getContext() {
        return mContext;
    }


    /**
     * Cambia entre los estados normal y de edición.
     */
    // TODO: nacimiento
    private void toggleEditMode() {
        FloatingActionButton fabEditar = (FloatingActionButton) findViewById(R.id.fab_editar);
        ImageView iv_foto = (ImageView) findViewById(R.id.perfil_image);

        if (inEditingMode) {    /** Cambiar a modo normal */
            // Modificar usuario con contenido de los campos; si hay errores de input cancelar
            if (!capturarInputPerfilUsuario()) {
                Toast.makeText(this, "Usuario no fue modificado", Toast.LENGTH_LONG)
                        .show();
                return;
            }

            // No permitir cambiar la foto
            if (iv_foto != null)
                iv_foto.setOnClickListener(null);

            // Esconder teclado
            View view = this.getCurrentFocus();
            if (view != null) {
                InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_IMPLICIT_ONLY);
            }

            Log.d(LOG_TAG, fetchedUser.toJSON());//
            // PUT-tear usuario posiblemente editado
            updateProfileInformation();

            // Cambio color del botón de edición
            if (fabEditar != null) {
                fabEditar.setBackgroundTintList(Utils.getColorStateList(this, R.color.editar_btn_inactive));
            }

            refreshProfileInformation(fetchedUserID);

        } else {    /** Cambiar a modo edición */
            // Cambia color del botón de edición
            if (fabEditar != null) {
                fabEditar.setBackgroundTintList(Utils.getColorStateList(this, R.color.editar_btn_active));
            }

            // Cambia funcionamiento del "volver" TODO

            // Permitir cambiar la foto
            if (iv_foto != null)
                iv_foto.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        // TODO: Emprolijar
                        final CharSequence[] options = {"Cámara", "Galería", "Cancelar"};
                        new AlertDialog.Builder(getContext())
                                .setTitle("Nueva imagen de perfil")
//                                .setMessage()
                                .setIcon(android.R.drawable.ic_input_add)
                                .setItems(options, new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        if (options[which] == "Cámara") {
                                            if (getPackageManager().hasSystemFeature(PackageManager.FEATURE_CAMERA))
                                                dispatchTakePictureIntent();
//                                            dialog.dismiss();// Hace falta?

                                        } else if (options[which] == "Galería") {
                                            dispatchChoosePictureIntent();
//                                            dialog.dismiss();// Hace falta?

                                        } else {
                                            dialog.dismiss();
                                        }
                                    }
                                })
                                .show();
                    }
                });

            // Precargar campos con valores actuales
            if (fetchedUser != null) {
                Utils.setTextViewText(this, R.id.text_perfil_nombre_editable, fetchedUser.getFirstName());
                Utils.setTextViewText(this, R.id.text_perfil_apellido_editable, fetchedUser.getLastName());
                Utils.setTextViewText(this, R.id.text_perfil_ciudad_editable, fetchedUser.getCity());
                Utils.setTextViewText(this, R.id.text_perfil_resumen_editable, fetchedUser.getSummary());
            }

            // Cargo autocompletado de JobPositions y Skills según SharedData
            populateAutoCompleteLists();

            List<Skill> skillsList = new ArrayList<>();
            if (fetchedUser != null)
                skillsList = fetchedUser.getSkills();
            mSkillAdapter = EditableListAdapter.populateEditableList(this,
                    (ListView) findViewById(R.id.perfil_skills_list_editable),
                    skillsList
            );
            ImageButton ib_skills =
                    (ImageButton) findViewById(R.id.boton_perfil_skill_agregar_item);
            if (ib_skills != null) {
                ib_skills.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        agregarSkill();
                    }
                });
            }

            List<Employment> employmentsList = new ArrayList<>();
            if (fetchedUser != null)
                employmentsList = fetchedUser.getWorkHistory();
            mJobsAdapter = EditableListAdapter.populateEditableList(this,
                    (ListView) findViewById(R.id.perfil_experiencia_laboral_list_editable),
                    employmentsList
            );
            ImageButton ib_workHistory =
                    (ImageButton) findViewById(R.id.boton_perfil_experiencia_laboral_agregar_item);
            if (ib_workHistory != null) {
                ib_workHistory.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        agregarEmployment();
                    }
                });
            }

            Utils.hideView(this, R.id.perfil_contactos_frame);
        }

        // Togglear la visibilidad de views pertinentes a cada modo
        for (@IdRes int vId : Utils.perfilVisibilityViews) {
            Utils.toggleViewVisibility(this, vId);
        }

        inEditingMode = !inEditingMode;
    }

    private boolean agregarEmployment() {
        EditText et_company = (EditText) findViewById(R.id.text_perfil_employment_new_company);
        AutoCompleteTextView et_position =
                (AutoCompleteTextView) findViewById(R.id.text_perfil_employment_new_position);
        EditText et_desde_mes =  (EditText) findViewById(R.id.perfil_employment_desde_mes);
        EditText et_desde_anio = (EditText) findViewById(R.id.perfil_employment_desde_anio);
        EditText et_hasta_mes =  (EditText) findViewById(R.id.perfil_employment_hasta_mes);
        EditText et_hasta_anio = (EditText) findViewById(R.id.perfil_employment_hasta_anio);

        if (et_company == null || et_company.length() == 0
                || et_position == null || et_position.length() == 0
                || et_desde_mes == null || et_desde_mes.length() == 0
                || et_desde_anio == null || et_desde_anio.length() == 0
                || et_hasta_mes == null || et_hasta_anio == null) {
            if (et_position != null)
                et_position.setError("Salvo 'hasta', ningún campo puede quedar vacío");
            return false;
        }
        et_position.setError(null);

        if ((et_hasta_mes.length() == 0 && et_hasta_anio.length() != 0)
            || et_hasta_mes.length() != 0 && et_hasta_anio.length() == 0) {
            et_position.setError("Para un trabajo actual, deje mes y año de 'hasta' vacíos");
            return false;
        }

        // "Hasta" puede estar vacío
        int hastaMes = (et_hasta_mes.length() == 0) ? 0 : Integer.valueOf(et_hasta_mes.getText().toString());
        int hastaAnio = (et_hasta_anio.length() == 0) ? 0 : Integer.valueOf(et_hasta_anio.getText().toString());

        try {
            Employment nuevoEmployment = Employment.create(this,
                            et_company.getText().toString(),
                            et_position.getText().toString(),
                            Integer.valueOf(et_desde_mes.getText().toString()),
                            Integer.valueOf(et_desde_anio.getText().toString()),
                            hastaMes, hastaAnio);
            if (nuevoEmployment == null) return false;
            mJobsAdapter.add(nuevoEmployment);
            et_company.setText(null);
            et_position.setText(null);
            et_desde_mes.setText(null);
            et_desde_anio.setText(null);
            et_hasta_mes.setText(null);
            et_hasta_anio.setText(null);
            mJobsAdapter.notifyDataSetChanged();

        } catch (IllegalArgumentException ex) {
            et_position.setError("Valor inválido para " + ex.getMessage());
            et_position.requestFocus();
            return false;
        }
        return true;
    }

    private boolean agregarSkill() {
        EditText et_skill = (EditText) findViewById(R.id.text_perfil_skill_new);
        // TODO: revisar también que esté en la lista de posibles
        if (et_skill == null || et_skill.length() == 0) return false;

        try {
            Skill nuevoSkill = Skill.create(this, et_skill.getText().toString());
            if (nuevoSkill == null) return false;
            mSkillAdapter.add(nuevoSkill);
            et_skill.setText("");
            mSkillAdapter.notifyDataSetChanged();

        } catch (IllegalArgumentException ex) {
            et_skill.setError("Skill desconocido");
            et_skill.requestFocus();
            return false;
        }

        return true;
    }

    // Si el input de algún campo es incorrecto, falla sin modificar al usuario.
    private boolean capturarInputPerfilUsuario() {
        User editedUser = new User(fetchedUser);

        if (!editedUser.setFirstName(Utils.getTextViewText(this, R.id.text_perfil_nombre_editable))) {
            Utils.editTextSetErrorAndFocus(this, R.id.text_perfil_nombre_editable,
                    "Nombre muy corto o muy largo");
            return false;
        }
        if (!editedUser.setLastName(Utils.getTextViewText(this, R.id.text_perfil_apellido_editable))) {
            Utils.editTextSetErrorAndFocus(this, R.id.text_perfil_apellido_editable,
                    "Apellido muy corto o muy largo");
            return false;
        }
        if (!editedUser.setCity(Utils.getTextViewText(this, R.id.text_perfil_ciudad_editable))) {
            Utils.editTextSetErrorAndFocus(this, R.id.text_perfil_ciudad_editable, "!");
            return false;
        }
        if (!editedUser.setSummary(Utils.getTextViewText(this, R.id.text_perfil_resumen_editable))) {
            Utils.editTextSetErrorAndFocus(this, R.id.text_perfil_resumen_editable,
                    "Máximo "+User.MAX_CHAR_SUMMARY+" caracteres");
            return false;
        }
        editedUser.setWorkHistory(mJobsAdapter.getList()); // TODO
        editedUser.setSkills(mSkillAdapter.getList());  // TODO

        fetchedUser = editedUser;
        return true;
    }


    private void modificarAmistad() {
        Utils.getJsonFromAppServer(getContext(), getString(R.string.get_contacts_path), fetchedUserID, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {

                ContactsResponse contactsResponse = ContactsResponse.parseJSON(response.toString());
                if (contactsResponse == null) {
                    Log.e(LOG_TAG, "Error de parseo de ContactsResponse");
                    return;
                }

                // Obtengo el estado de amistad del usuario fetched con el connected.
                Contact.Status estado = contactsResponse.getStatusForId(connectedUserID);

                switch (estado) {
                    case NONE:
                        Utils.confirmarAccion(getContext(), "Solicitud", getString(R.string.dialog_agregar_contacto),
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        // TODO: Enviar solicitud
//                                        Gson gson = new GsonBuilder()
//                                                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
//                                                .create();
//                                        String json = gson.toJson(new Solicitud(fetchedUserID, Contact.Status.REQUESTED));
                                        // TODO: postear json
                                    }
                                });
                        break;
                    case REQUESTED:
                        // Confirmar y cancelar?
                        break;
                    case RECEIVED:
                        Utils.confirmarAccion(getContext(), "Solicitud", getString(R.string.dialog_aceptar_contacto),
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
//                                        Gson gson = new GsonBuilder()
//                                                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
//                                                .create();
//                                        String json = gson.toJson(new Solicitud(fetchedUserID, Contact.Status.ACTIVE));
                                        // TODO: Aceptar solicitud
                                    }
                                });
                        break;
                    case ACTIVE:
                        // Confirmar y cancelar amistad
                        break;
                    default:
                        Log.e(LOG_TAG, "This is not possible...");  // TODO: Revisar contra NONE
                }

                colorearBotonAmistad(estado);
            }
        }, LOG_TAG);
    }

    private void colorearBotonAmistad(Contact.Status estado) {

        FloatingActionButton fab_amigar = (FloatingActionButton) findViewById(R.id.fab_amigar);
        if (fab_amigar == null) {
            Log.e(LOG_TAG, "No pude encontrar fab_amigar");
            return;
        }
        ColorStateList csl;

        switch (estado) {
           case REQUESTED:
                csl = Utils.getColorStateList(this, R.color.amigar_btn_requested);
                break;
            case RECEIVED:
                csl = Utils.getColorStateList(this, R.color.amigar_btn_received);
                break;
            case ACTIVE:
                csl = Utils.getColorStateList(this, R.color.amigar_btn_active);
                break;
            default:  // TODO: Revisar contra NONE
                fab_amigar.setEnabled(false);
            case NONE:
                csl = Utils.getColorStateList(this, R.color.amigar_btn_none);
        }
        fab_amigar.setBackgroundTintList(csl);
    }

    public void refreshProfileInformation(final long idFetched) {

        Utils.getJsonFromAppServer(getContext(), getString(R.string.perfil_get_user_path), idFetched,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {

                        User mUser = User.parseJSON(response.toString());
                        if (mUser != null) {

                            fetchedUser = mUser;
                            fillProfile(mUser);

                        } else {
                            Log.e(LOG_TAG, "Error de parseo de usuario, no puedo llenar el perfil");
                        }
                    }
                }, LOG_TAG);
    }


    public void cargarFotoDePerfil(final long idFetched) {

        // TODO / DE PRUEBA
        if (mCurrentPhotoPath != null) {
            ImageView imageView = (ImageView) findViewById(R.id.perfil_image);
            if (imageView != null) {
                try {
                    Bitmap imageBitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), mPhotoURI);
                    imageView.setImageBitmap(imageBitmap);
                    return;
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        }

        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(this)).buildUpon()
                .appendPath(getString(R.string.perfil_get_photo_path))
                .appendPath(Long.toString(idFetched))
                .build();
        final String url = builtUri.toString();

        Utils.cargarImagenDeURLenImageView(getApplicationContext(),
                (ImageView) findViewById(R.id.perfil_image),
                url, LOG_TAG);
    }

    private void fillProfile(User mUser) {
        Utils.showView(this, R.id.perfil_information_layout);

        collapsingToolbarLayout.setTitle(mUser.getFullName());

        Utils.setTextViewText(this, R.id.text_perfil_mail, mUser.getEmail());
        Utils.setTextViewText(this, R.id.text_perfil_ciudad, mUser.getCity());
        Utils.setTextViewText(this, R.id.text_perfil_nacimiento, mUser.getLineaNacimiento());
        Utils.setTextViewText(this, R.id.text_perfil_resumen, mUser.getSummary(), true);
        Utils.setTextViewText(this, R.id.text_perfil_trabajo_actual, mUser.getTrabajosActuales(), true);

        Utils.populateStringList(this, R.id.perfil_experiencia_laboral_list, mUser.getListaJobs());
        Utils.populateStringList(this, R.id.perfil_skills_list, mUser.getListaSkills());

        Utils.getJsonFromAppServer(getContext(), getString(R.string.get_contacts_path), fetchedUserID,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        ContactsResponse cs = ContactsResponse.parseJSON(response.toString());
                        if (cs != null) populateContacts(cs);
                    }
                }, LOG_TAG);
    }


    public void updateProfileInformation() {

        if (fetchedUserID != connectedUserID)
            return;

        JSONObject jsonObject = fetchedUser.toJsonObject();

        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(this)).buildUpon()
                .appendPath(getString(R.string.put_user_path))
                .appendPath(Long.toString(fetchedUserID))
                .build();
        String url = builtUri.toString();

        Utils.fetchJsonFromUrl(getContext(), Request.Method.PUT, url, jsonObject,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        if (response != null) {
                            Log.d(LOG_TAG, "User PUT Response: "+response.toString());
                        }
                    }
                }, LOG_TAG);

//            @Override
//            public byte[] getBody() throws AuthFailureError {
//                try {
//                    return mRequestBody == null ? null : mRequestBody.getBytes("utf-8");
//                } catch (UnsupportedEncodingException uee) {
//                    VolleyLog.wtf("Unsupported Encoding while trying to get the bytes of %s using %s",
//                            mRequestBody, "utf-8");
//                    return null;
//                }
//            }

//            @Override
//            protected Response<String> parseNetworkResponse(NetworkResponse response) {
//                String responseString = "";
//                if (response != null) {
//                    responseString = String.valueOf(response.statusCode);
//                    // can get more details such as response.headers
//                }
//                return Response.success(responseString, HttpHeaderParser.parseCacheHeaders(response));
//            }

    }

    private void dispatchChoosePictureIntent() {
        Intent getIntent = new Intent(Intent.ACTION_GET_CONTENT);
        getIntent.setType("image/*");

        Intent pickIntent = new Intent(Intent.ACTION_PICK,
                android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        pickIntent.setType("image/*");

        Intent chooserIntent = Intent.createChooser(getIntent, "Select Image");
        chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, new Intent[] {pickIntent});

        if (chooserIntent.resolveActivity(getPackageManager()) != null) {
                startActivityForResult(chooserIntent, REQUEST_PICK_IMAGE);
        }
    }

    private void dispatchTakePictureIntent() {
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            File photoFile;
            try {
                photoFile = createImageFile("new_profile");

            } catch (IOException ex) {
                // Error occured while creating the File
                Log.e(LOG_TAG, "Error al intentar crear el archivo de foto.");
                return;
            }
            if (photoFile != null) {
                mPhotoURI = FileProvider.getUriForFile(this,
                                                    "ar.fiuba.jobify.fileprovider",
                                                    photoFile);

                takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, mPhotoURI);
                startActivityForResult(takePictureIntent, REQUEST_TAKE_PHOTO);
            }
        }
    }

    private File createImageFile(String imageFileName) throws IOException {
//        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss", Locale.US)
//                .format(new Date());
//        String imageFileName = "JPEG_" + timeStamp + "_";

        File storageDir = getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        if (storageDir == null)
            throw new IOException("getExternalFilesDir dio error");
//        File imageFile = File.createTempFile(imageFileName, ".jpg", storageDir); TODO: REVISAR SI VOLVER A ESTO
        File imageFile = new File(storageDir, imageFileName + ".jpg");

        mCurrentPhotoPath = imageFile.getAbsolutePath();//"file:" + imageFile.getAbsolutePath();//"content:" ?
        return imageFile;
    }

    // TODO: Emprolijar, extraer, generalizar.
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if ((requestCode == REQUEST_TAKE_PHOTO || requestCode == REQUEST_PICK_IMAGE)
                && resultCode == RESULT_OK) {

            File imageFile;

            if (requestCode == REQUEST_TAKE_PHOTO) {
                imageFile = new File(mCurrentPhotoPath);
                Log.d(LOG_TAG, "Absolute path al sacar una foto: "+mCurrentPhotoPath);

                try {
                    Bitmap bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), mPhotoURI);

                    bitmap = normalizarBitmap(bitmap);
                    imageFile = guardarImagen(bitmap);

                } catch (IOException ex) {
                    ex.printStackTrace();
                }

            } else {        // REQUEST_PICK_IMAGE
                Uri imageUri = data.getData();
                if (imageUri == null) {
                    Log.d(LOG_TAG, "Hay que conseguir la imagen de otra forma");
                    return;
                }

                try {
                    Log.d(LOG_TAG, "imageUri al elegir una imagen: "+imageUri);
                    mPhotoURI = imageUri;

                    Bitmap bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), imageUri);
                    bitmap = normalizarBitmap(bitmap);

                    imageFile = guardarImagen(bitmap);

                    if (imageFile == null) {
                        Log.d(LOG_TAG, "Imagen guardada con éxito");
                        return;
                    } else {
                        Log.d(LOG_TAG, "Problema al guardar la imagen");
                    }

                } catch (IOException ex) {
                    return;
                }
            }

            Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(this)).buildUpon()
                    .appendPath(getString(R.string.perfil_post_photo_path))
                    .appendPath(Long.toString(fetchedUserID))
                    .build();
            final String url = builtUri.toString();

            if (imageFile != null && imageFile.exists()) {
                Utils.PhotoMultipartRequest<String> imageUploadReq =
                        new Utils.PhotoMultipartRequest<>(url, imageFile, new Response.Listener<String>() {
                            @Override
                            public void onResponse(String response) {
                                Log.d(LOG_TAG, "Response correcta i guess. " + response);
                            }
                        }, new Response.ErrorListener() {
                            @Override
                            public void onErrorResponse(VolleyError error) {
                                Log.e(LOG_TAG, "Volley image post error");
                                if (error.networkResponse != null)
                                    Log.e(LOG_TAG, "Status code: " + error.networkResponse.statusCode);
                                error.printStackTrace();
                            }
                        });

                try {
                    Log.d(LOG_TAG, "Headers: " + imageUploadReq.getHeaders().toString());//
                    Log.d(LOG_TAG, "BodyContentType: " + imageUploadReq.getBodyContentType());//
                } catch (AuthFailureError er) {
                    Log.d(LOG_TAG, "AuthFailureError in test Logs");//
                    er.printStackTrace();
                }
                RequestQueueSingleton.getInstance(this)
                        .addToRequestQueue(imageUploadReq);
            } else {
                Log.w(LOG_TAG, "Archivo en " + mPhotoURI.getPath() + " no existe.");
            }
        }
    }

    private Bitmap normalizarBitmap(Bitmap bitmap) {
        int newWidth = bitmap.getWidth() > 1000 ? 1000 : bitmap.getWidth(); // HARDCODEO
        int newHeight = (int) Math.round((1.0 * bitmap.getHeight() / bitmap.getWidth()) * newWidth);
        return Bitmap.createScaledBitmap(bitmap, newWidth, newHeight, true);
    }

    private File guardarImagen(Bitmap bitmap) {
        boolean success = false;
        File imageFile = null;
        try {
            imageFile = createImageFile("new_profile");

            FileOutputStream outStream = new FileOutputStream(imageFile);

            // Pierdo calidad ;//
            bitmap.compress(Bitmap.CompressFormat.JPEG, 50, outStream);

            outStream.flush();
            outStream.close();
            success = true;

        } catch (IOException ex) {
            ex.printStackTrace();
        }

        if (!success)
            return null;
        return imageFile;
    }


    private void populateContacts(ContactsResponse response) {

        // Obtengo el estado de amistad del usuario fetched con el connected para colorear el botón
        Contact.Status estado = response.getStatusForId(connectedUserID);
        colorearBotonAmistad(estado);

        ArrayList<Contact> contacts = response.getContactsWithStatus(Contact.Status.ACTIVE);

        /// TODO: hardcodeado, de prueba
//        ArrayList<Contact> contacts = new ArrayList<>();
//        contacts.add(new Contact(1, "John", "Roberts", new Employment("CEO", "FBI"), Contact.Status.ACTIVE));
//        contacts.add(new Contact(2, "Joan", "Roberts", new Employment("ZEO", "NSA"), Contact.Status.ACTIVE));
//        contacts.add(new Contact(3, "Sean", "Roberts", new Employment("CTO", "FBI"), Contact.Status.ACTIVE));
//        contacts.add(new Contact(4, "Jon", "Roberts", new Employment("CFO", "GitHub"), Contact.Status.ACTIVE));
//        contacts.add(new Contact(5, "Jone", "Roberts", new Employment("Toilet cleaner", "Zoo"), Contact.Status.ACTIVE));
//        contacts.add(new Contact(6, "Mark", "Zuckerberg", new Employment("CEO", "Facebook"), Contact.Status.REQUESTED));
//        // Solo mostrar contactos activos TODO: BORRAR
//        ArrayList<Contact> activeContacts = new ArrayList<>();
//        for (Contact c : contacts) {
//            if (c.getStatus().equals(Contact.Status.ACTIVE))
//                activeContacts.add(c);
//        }
        ///

        if (contacts.size() == 0) {
            Utils.hideView(this, R.id.perfil_contactos_frame);
            return;
        } else {
            Utils.showView(this, R.id.perfil_contactos_frame);
        }

        HorizontalListView mHLView = (HorizontalListView) findViewById(R.id.perfil_contactos_list);
        if (mHLView != null) {

            final ContactCardAdapter mAdapter = new ContactCardAdapter(contacts);
            mHLView.setAdapter(mAdapter);
            mHLView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                    Contact clickedUser = (Contact) mAdapter.getItem(position);
                    startActivity(
                            new Intent(PerfilActivity.this, PerfilActivity.class)
                                    .putExtra(FETCHED_USER_ID_MESSAGE, clickedUser.getId())
                    );
                }
            });

            TextView tv_cantContactos = (TextView) findViewById(R.id.text_perfil_cant_contactos);
            if (tv_cantContactos != null) {
                tv_cantContactos.setText(String.valueOf(mAdapter.getCount()));
            }

        } else {
            Log.e(LOG_TAG, "No se encontró el gridview de contactos!");
        }
    }

    // TODO: refactorizar?
    private void populateAutoCompleteLists() {

        try {
            AutoCompleteTextView et_employment =
                    (AutoCompleteTextView) findViewById(R.id.text_perfil_employment_new_position);
            List<JobPosition> jobPositions = SharedDataSingleton.getInstance(this).getJobPositions();

            if (jobPositions != null && et_employment != null) {
                ArrayList<String> jpArray = new ArrayList<>();
                for (JobPosition jp : jobPositions) {
                    jpArray.add(jp.getNombre());
                }
                ArrayAdapter<String> employmentsAdapter = new ArrayAdapter<>(this,
                        android.R.layout.simple_dropdown_item_1line, jpArray);
                et_employment.setAdapter(employmentsAdapter);
                Log.d(LOG_TAG, "Setteado el autocomplete adapter de Employments");
            }
        } catch (SharedDataSingleton.NoDataException ex) {
            Toast.makeText(getContext(), "Problemas con SS.JobPositions", Toast.LENGTH_LONG)
                    .show();
        }

        try {
            AutoCompleteTextView et_skill =
                    (AutoCompleteTextView) findViewById(R.id.text_perfil_skill_new);
            List<Skill> skills = SharedDataSingleton.getInstance(this).getSkills();

            if (skills != null && et_skill != null) {
                ArrayList<String> skArray = new ArrayList<>();
                for (Skill sk : skills) {
                    skArray.add(sk.getNombre());
                }
                ArrayAdapter<String> skillsAdapter = new ArrayAdapter<>(this,
                        android.R.layout.simple_dropdown_item_1line, skArray);
                et_skill.setAdapter(skillsAdapter);
                Log.d(LOG_TAG, "Setteado el autocomplete adapter de Skills");
            }
        } catch (SharedDataSingleton.NoDataException ex) {
            Toast.makeText(getContext(), "Problemas con SS.Skills", Toast.LENGTH_LONG)
                    .show();
        }
    }


    private class ContactCardAdapter extends BaseAdapter {

        ArrayList<Contact> mContacts;

        public ContactCardAdapter(List<Contact> contactList) {
            mContacts = new ArrayList<>(contactList);
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
                itemView = LayoutInflater.from(getContext())
                        .inflate(R.layout.contact_card, parent, false);
            }

            Contact contact = (Contact) getItem(position);
            if (contact != null) {

                Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(getContext())).buildUpon()
                        .appendPath(getString(R.string.perfil_get_thumbnail_path))
                        .appendPath(Long.toString(contact.getId()))
                        .build();
                Utils.cargarImagenDeURLenImageView(getApplicationContext(),
                        (ImageView) findViewById(R.id.contact_card_foto),
                        builtUri.toString(), LOG_TAG+"CC");

                TextView tv_nombre  = (TextView) itemView.findViewById(R.id.contact_card_nombre);
                if (tv_nombre != null)
                    tv_nombre.setText(contact.getFullName());

                TextView tv_trabajo = (TextView) itemView.findViewById(R.id.contact_card_trabajo);
                if (tv_trabajo != null)
                    tv_trabajo.setText(contact.getCurrentJob().getOneLiner());
            }

            return itemView;
        }
    }
}
