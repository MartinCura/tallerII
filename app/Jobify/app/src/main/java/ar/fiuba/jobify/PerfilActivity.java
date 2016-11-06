package ar.fiuba.jobify;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.location.Location;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.IdRes;
import android.support.annotation.LayoutRes;
import android.support.design.widget.CollapsingToolbarLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.DialogFragment;
import android.support.v4.content.FileProvider;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
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
import ar.fiuba.jobify.app_server_api.Recommendation;
import ar.fiuba.jobify.app_server_api.Solicitud;
import ar.fiuba.jobify.app_server_api.User;
import ar.fiuba.jobify.shared_server_api.Skill;
import ar.fiuba.jobify.utils.PerfilUtils;

public class PerfilActivity extends NavDrawerActivity {

    private final String LOG_TAG = PerfilActivity.class.getSimpleName();

    public final static String FETCHED_USER_ID_MESSAGE = "ar.fiuba.jobify.FETCHED_USER_ID_MESSAGE";
    public final static String PERFIL_MODE_MESSAGE = "ar.fiuba.jobify.PERFIL_MODE_MESSAGE";
    private long fetchedUserID = 2;
    private User fetchedUser;

    String mCurrentPhotoPath = null;
    Uri mPhotoURI;

    private static Context mContext;

    private CollapsingToolbarLayout collapsingToolbarLayout;//
    private boolean inEditingMode = false;  // TODO: revisar qué ocurre si giro la pantalla
    private EditableListAdapter<Skill> mSkillAdapter;
    private EditableListAdapter<Employment> mJobsAdapter;

    private PerfilUtils.MyLocationService mLocationListener = null;


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
            fabAmigar.setBackgroundTintList(Utils.getColorStateList(this, R.color.amigar_btn_none));
            fabAmigar.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (fetchedUser != null && fetchedUser.getId() != connectedUserID)
                        modificarAmistad();
                }
            });
        }

        FloatingActionButton fabRecomendar = (FloatingActionButton) findViewById(R.id.fab_recomendar);
        if (fabRecomendar != null) {
            fabRecomendar.setBackgroundTintList(Utils.getColorStateList(this, R.color.recomendar_btn_false));
            fabRecomendar.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    toggleRecomendar(fetchedUserID);
                }
            });
        }

        FloatingActionButton fabChatear = (FloatingActionButton) findViewById(R.id.fab_chatear);
        if (fabChatear != null)
            fabChatear.setBackgroundTintList(Utils.getColorStateList(this, R.color.chatear_btn));

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

        PerfilUtils.populateAutoCompleteLists(this);

        if (fetchedUserID == connectedUserID) {
            if (fabAmigar != null) fabAmigar.setVisibility(View.GONE);
            if (fabRecomendar != null) fabRecomendar.setVisibility(View.GONE);
            if (fabChatear != null) fabChatear.setVisibility(View.GONE);
            if (fabEditar != null) fabEditar.setVisibility(View.VISIBLE);
        }

        Utils.hideView(this, R.id.perfil_information_layout);
        PerfilUtils.showProgress(this, true);
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
        if (intent != null && intent.hasExtra(PERFIL_MODE_MESSAGE)) {

            boolean empezarEnModoEdicion = intent.getBooleanExtra(PERFIL_MODE_MESSAGE, false);
            if (empezarEnModoEdicion) {

                PerfilUtils.showProgress(this, false);
                Utils.showView(this, R.id.perfil_information_layout);

                inEditingMode = false;
                toggleEditMode();
                startLocationService(null);
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

    public PerfilActivity getActivity() {
        return this;
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

            // Frena MyLocationService si estaba corriendo, actualizando coordenadas en dicho caso
            finishLocationService();

            // No permitir cambiar la foto
            if (iv_foto != null)
                iv_foto.setOnClickListener(null);

            // Esconder teclado
            View view = this.getCurrentFocus();
            if (view != null) {
                InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_IMPLICIT_ONLY);
            }

            Log.d(LOG_TAG, "User PUT Request: " + fetchedUser.toJson());//
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
                                            PerfilUtils.dispatchChoosePictureIntent(getActivity());
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
                Utils.setTextViewText(this, R.id.perfil_nacimiento_dia, String.valueOf(fetchedUser.getDiaNacimiento()));
                Utils.setTextViewText(this, R.id.perfil_nacimiento_mes, String.valueOf(fetchedUser.getMesNacimiento()));
                Utils.setTextViewText(this, R.id.perfil_nacimiento_anio, String.valueOf(fetchedUser.getAnioNacimiento()));
            }

            // Cargo autocompletado de JobPositions y Skills según SharedData
            PerfilUtils.populateAutoCompleteLists(this);

            final PerfilActivity activity = this;

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
                        PerfilUtils.agregarSkill(activity, mSkillAdapter);
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
                        PerfilUtils.agregarEmployment(activity, mJobsAdapter);
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
        if (!editedUser.setDateOfBirth( Utils.getTextViewInt(this, R.id.perfil_nacimiento_dia),
                                        Utils.getTextViewInt(this, R.id.perfil_nacimiento_mes),
                                        Utils.getTextViewInt(this, R.id.perfil_nacimiento_anio) )) {
            Utils.editTextSetErrorAndFocus(this, R.id.perfil_nacimiento_anio, "Fecha inválida");
            return false;
        }
        editedUser.setWorkHistory(mJobsAdapter.getList()); // TODO
        editedUser.setSkills(mSkillAdapter.getList());  // TODO

        fetchedUser = editedUser;
        return true;
    }


    private void modificarAmistad() {
        Utils.getJsonFromAppServer(getContext(), getString(R.string.get_contacts_path), connectedUserID, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {

                ContactsResponse contactsResponse = ContactsResponse.parseJSON(response.toString());
                if (contactsResponse == null) {
                    Log.e(LOG_TAG, "Error de parseo de ContactsResponse");
                    return;
                }

                // Obtengo el estado de amistad del usuario fetched con el connected.
                Contact.Status estado = contactsResponse.getStatusForId(fetchedUserID);

                switch (estado) {
                    case NONE:
                        Utils.confirmarAccion(getContext(), "Solicitud", getString(R.string.dialog_agregar_contacto),
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        enviarModificacionDeAmistad(Solicitud.Action.ADD, "Solicitud enviada");
                                    }
                                });
                        break;
                    case REQUESTED:
                        Utils.confirmarAccion(getContext(), "Solicitud", getString(R.string.dialog_cancelar_contacto),
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        enviarModificacionDeAmistad(Solicitud.Action.UNFRIEND, "Solicitud cancelada");
                                    }
                                });
                        break;
                    case RECEIVED:
                        Utils.confirmarAccion(getContext(), "Solicitud", getString(R.string.dialog_aceptar_contacto),
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        enviarModificacionDeAmistad(Solicitud.Action.ACCEPT, "Solicitud aceptada");
                                    }
                                }, new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        enviarModificacionDeAmistad(Solicitud.Action.UNFRIEND, "Solicitud rechazada");
                                    }
                                }, R.string.dialog_no);
                        break;
                    case ACTIVE:
                        Utils.confirmarAccion(getContext(), "Solicitud", getString(R.string.dialog_eliminar_contacto),
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        enviarModificacionDeAmistad(Solicitud.Action.UNFRIEND, "Contacto eliminado");
                                    }
                                });
                        break;
                    default:
                        Log.e(LOG_TAG, "This is not possible...");  // TODO: Revisar contra NONE
                }
                PerfilUtils.colorearBotonAmistad(getActivity(), estado);
            }
        }, LOG_TAG);
    }

    public void enviarModificacionDeAmistad(Solicitud.Action action, final String responseStr) {
        JSONObject jsonRequest =
                new Solicitud(connectedUserID, fetchedUserID, action)
                        .toJsonObject();
        Utils.putJsonToAppServer(getContext(), getString(R.string.put_contacts_path),
                jsonRequest, new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        // TODO: Ignorando response, algo que hacer con ello?
                        Toast.makeText(PerfilActivity.this, responseStr, Toast.LENGTH_LONG)
                                .show();
                    }
                }, LOG_TAG);
    }



    public void refreshProfileInformation(final long idFetched) {

        Utils.getJsonFromAppServer(getContext(), getString(R.string.get_user_path), idFetched,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {

                        User mUser = User.parseJSON(response.toString());
                        if (mUser != null) {

                            fetchedUser = mUser;
                            Log.d(LOG_TAG, "Fetched user: "+response.toString());//
                            fillProfile(mUser);

                        } else {
                            Log.e(LOG_TAG, "Error de parseo de usuario, no puedo llenar el perfil");
                        }
                    }
                }, LOG_TAG);
    }


    public void cargarFotoDePerfil(final long idFetched) {

        // De muy poco uso
//        if (mCurrentPhotoPath != null) {
//            ImageView imageView = (ImageView) findViewById(R.id.perfil_image);
//            if (imageView != null) {
//                try {
//                    Bitmap imageBitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), mPhotoURI);
//                    imageView.setImageBitmap(imageBitmap);
//                    return;
//                } catch (IOException ex) {
//                    ex.printStackTrace();
//                    Log.e(LOG_TAG, "Error en cargado de imagen de perfil");
//                }
//            }
//        }

        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(this)).buildUpon()
                .appendPath(getString(R.string.get_photo_path))
                .appendPath(Long.toString(idFetched))
                .build();
        final String url = builtUri.toString();

        Utils.cargarImagenDeURLenImageView(getApplicationContext(),
                (ImageView) findViewById(R.id.perfil_image),
                url, LOG_TAG);
    }

    private void fillProfile(User mUser) {
        PerfilUtils.showProgress(this, false);
        Utils.showView(this, R.id.perfil_information_layout);

        collapsingToolbarLayout.setTitle(mUser.getFullName());

        Utils.setTextViewText(this, R.id.text_perfil_mail, mUser.getEmail());
        Utils.setTextViewText(this, R.id.text_perfil_ciudad, mUser.getCity());
        Utils.setTextViewText(this, R.id.text_perfil_cant_recomendaciones,
                Long.toString(mUser.getCantRecomendaciones()) + " recomendaciones");//semi hardcode
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
                        if (cs != null)
                            PerfilUtils.populateContacts(getActivity(), cs, connectedUserID);
                    }
                }, LOG_TAG);

        PerfilUtils.colorearBotonRecomendar(this, mUser.fueRecomendadoPor(connectedUserID));
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
                            Log.d(LOG_TAG, "User PUT Response: " + response.toString());
                        }
                    }
                }, LOG_TAG);
    }

    public void dispatchTakePictureIntent() {
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
                startActivityForResult(takePictureIntent, PerfilUtils.REQUEST_TAKE_PHOTO);
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

        if ( (requestCode == PerfilUtils.REQUEST_TAKE_PHOTO
           || requestCode == PerfilUtils.REQUEST_PICK_IMAGE)
                && resultCode == RESULT_OK) {

            File imageFile; // TODO: sacar repetición

            if (requestCode == PerfilUtils.REQUEST_TAKE_PHOTO) {
                imageFile = new File(mCurrentPhotoPath);
                Log.d(LOG_TAG, "Absolute path al sacar una foto: "+mCurrentPhotoPath);

                try {
                    Bitmap bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), mPhotoURI);

                    bitmap = Utils.normalizarBitmap(bitmap);
                    imageFile = guardarImagen(bitmap);

                } catch (IOException ex) {
                    ex.printStackTrace();
                    Log.e(LOG_TAG, "Error en cargado de imagen sacada");
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
                    bitmap = Utils.normalizarBitmap(bitmap);

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

            final String url = Utils.getAppServerUrl(this, fetchedUserID,
                                getString(R.string.post_photo_path));

            if (imageFile != null && imageFile.exists()) {
                try {
                    Utils.PhotoMultipartRequest<String> imageUploadReq =
                            new Utils.PhotoMultipartRequest<>(getContext(), url, imageFile,
                                    new Response.Listener<String>() {
                                        @Override
                                        public void onResponse(String response) {
                                            Log.d(LOG_TAG, "Response correcta i guess. " + response);
                                        }
                                    }, new Response.ErrorListener() {
                                        @Override
                                        public void onErrorResponse(VolleyError error) {
                                            Log.e(LOG_TAG, "Volley image post error");
                                            if (error.networkResponse != null)
                                                if (error.networkResponse.statusCode == 403)
                                                    Log.d(LOG_TAG, error.networkResponse.statusCode
                                                            + " FORBIDDEN");
                                                else
                                                    Log.e(LOG_TAG, "Status code: "
                                                            + error.networkResponse.statusCode);
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
                } catch (IOException ex) {
                    ex.printStackTrace();
                    Log.e(LOG_TAG, "Error en subido de imagen.");
                }
            } else {
                Log.w(LOG_TAG, "Archivo en " + mPhotoURI.getPath() + " no existe.");
            }

        } else if (requestCode == PerfilUtils.REQUEST_GET_LOCATION && resultCode == RESULT_OK) {
            startLocationService(null);
        }
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
            Log.e(LOG_TAG, "Error en guardado de imagen");
        }

        if (!success)
            return null;
        return imageFile;
    }


    public void startLocationService(View v) {
        Toast.makeText(this, "Actualizando ubicación", Toast.LENGTH_SHORT)
                .show();
        // TODO: Check GPS is on?
        try {
            mLocationListener =
                    new PerfilUtils.MyLocationService(this, R.id.text_perfil_ciudad_editable);
        } catch (SecurityException ex) {
            mLocationListener = null;
        }
    }

    public void finishLocationService() {
        if (mLocationListener == null)
            return;

        Location location = mLocationListener.getLocation();
        if (location != null)
            fetchedUser.setLocacion(location.getLatitude(), location.getLongitude());

        mLocationListener.finish();
    }


    public void showDatePickerDialog(View v) {
        DialogFragment newFragment = new PerfilUtils.DatePickerFragment();
        newFragment.show(getSupportFragmentManager(), "datePicker");
    }


    private void toggleRecomendar(final long recomendadoID) {

        final boolean yaRecomendado = fetchedUser.fueRecomendadoPor(connectedUserID);
        final int method;
        final String url, mensajeDialogo;
        final Recommendation recom = new Recommendation(connectedUserID, recomendadoID);

        if (!yaRecomendado) {
            method = Request.Method.PUT;
            String path = getString(R.string.as_make_recommendation_path);
            url = Utils.getAppServerUrl(getContext(), path);
            mensajeDialogo = getString(R.string.confirmacion_recomendar);
        } else {
            method = Request.Method.DELETE;
            String path = getString(R.string.as_cancel_recommendation_path);
            // En el caso de DELETE, lo que realmente importa son los queries en la url
            url = Utils.getAppServerUrl(getContext(), path, recom.toQueries());
            mensajeDialogo = getString(R.string.confirmacion_desrecomendar);
        }

        Utils.confirmarAccion(getContext(), "Recomendación", mensajeDialogo,
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Utils.fetchJsonFromUrl(getContext(), method, url, recom.toJsonObject(),
                                new Response.Listener<JSONObject>() {
                                    @Override
                                    public void onResponse(JSONObject response) {
                                        // TODO: No chequea que el status sea 200, será inmediato?

                                        boolean ahoraRecomendado = !yaRecomendado;
                                        PerfilUtils.colorearBotonRecomendar(getActivity(), ahoraRecomendado);

                                        // TODO: Mover a R.string
                                        if (ahoraRecomendado) {
                                            Toast.makeText(getContext(), "¡Usuario recomendado!",
                                                    Toast.LENGTH_LONG).show();
                                        } else {
                                            Toast.makeText(getContext(), "Se ha quitado la recomendación :(",
                                                    Toast.LENGTH_LONG).show();
                                        }
                                        refreshProfileInformation(fetchedUserID);
                                    }
                                }, LOG_TAG);
                    }
                });
    }


    public void irAConversacion(View v) {
        Utils.iniciarConversacionActivity(this, fetchedUserID);
    }
}
