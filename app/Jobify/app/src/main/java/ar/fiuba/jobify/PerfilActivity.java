package ar.fiuba.jobify;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.IdRes;
import android.support.annotation.LayoutRes;
import android.support.design.widget.CollapsingToolbarLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.content.FileProvider;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.ParseError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.VolleyLog;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.app_server_api.Employment;
import ar.fiuba.jobify.app_server_api.User;
import ar.fiuba.jobify.shared_server_api.Skill;

public class PerfilActivity extends NavDrawerActivity {

    private final String LOG_TAG = PerfilActivity.class.getSimpleName();

    public final static String FETCHED_USER_ID_MESSAGE = "ar.fiuba.jobify.FETCHED_USER_ID_MESSAGE";
    private long fetchedUserID = 2;
    private User fetchedUser;

    static final int REQUEST_TAKE_PHOTO = 1;
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
            fabEditar.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (fetchedUser != null)
                        switchEditMode();
                }
            });
        }

        if (fetchedUserID == connectedUserID) {
            if (fabAmigar != null) fabAmigar.setVisibility(View.GONE);
            if (fabRecomendar != null) fabRecomendar.setVisibility(View.GONE);
            if (fabChatear != null) fabChatear.setVisibility(View.GONE);
            if (fabEditar != null) fabEditar.setVisibility(View.VISIBLE);
        }
    }

    @Override
    public void setContentView(@LayoutRes int layoutResID) {
        super.setContentView(layoutResID);
        onCreateDrawer();
    }

    @Override
    protected void onResume() {
        super.onResume();

        refreshProfileInformation(fetchedUserID);
        cargarFotoDePerfil(fetchedUserID);
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

    public static Context getContext() {
        return mContext;
    }


    /**
     * Cambia entre los estados normal y de edición.
     */
    // TODO: nacimiento
    private void switchEditMode() {
        FloatingActionButton fabEditar = (FloatingActionButton) findViewById(R.id.fab_editar);
        ImageView iv_foto = (ImageView) findViewById(R.id.perfil_image);

        if (inEditingMode) {    /** Cambiar a modo normal */
            // Modificar usuario con contenido de los campos; si hay errores de input cancelar
            if (!capturarInputPerfilUsuario()) {
                Toast.makeText(this, "Usuario no modificado", Toast.LENGTH_LONG)
                        .show();
                return;
            }

            // No permitir cambiar la foto
            if (iv_foto != null)
                iv_foto.setOnClickListener(null);

            // Esconder teclado
            View view = this.getCurrentFocus();
            if (view != null) {
                InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_IMPLICIT_ONLY);
            }

            Log.d(LOG_TAG, fetchedUser.toJSON());//
            // Postear usuario editado TODO
//            updateProfileInformation(fetchedUser);

            // Cambio color del botón de edición
//            if (fabEditar != null)
//                fabEditar.setBackgroundTintList(null);

            refreshProfileInformation(fetchedUserID);

        } else {    /** Cambiar a modo edición */
            // Cambia color del botón de edición
            if (fabEditar != null) {
//                ColorStateList csl = new ColorStateList(
//                        new int[][] { new int[] { android.R.attr.state_enabled } },
//                        new int[] { Color.GREEN });
////                fabEditar.setBackgroundTintList(csl);
                fabEditar.setBackgroundColor(Color.BLUE);
            }

            // Cambia funcionamiento del "volver" TODO

            // Permitir cambiar la foto
            if (iv_foto != null)
                iv_foto.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (getPackageManager().hasSystemFeature(PackageManager.FEATURE_CAMERA))
                            dispatchTakePictureIntent();
                    }
                });

            // Precargar campos con valores actuales
            Utils.setTextViewText(this, R.id.text_perfil_nombre_editable, fetchedUser.getFirstName());
            Utils.setTextViewText(this, R.id.text_perfil_apellido_editable, fetchedUser.getLastName());
            Utils.setTextViewText(this, R.id.text_perfil_ciudad_editable, fetchedUser.getCity());
            Utils.setTextViewText(this, R.id.text_perfil_resumen_editable, fetchedUser.getSummary());

            mSkillAdapter = EditableListAdapter.populateEditableList(this,
                    (ListView) findViewById(R.id.perfil_skills_list_editable),
                    fetchedUser.getSkills()
            );
            ImageButton ib_skills =
                    (ImageButton) findViewById(R.id.boton_perfil_skill_agregar_item);
            if (ib_skills != null) {
                ib_skills.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        EditText et_skill = (EditText) findViewById(R.id.text_perfil_skill_new);
                        if (et_skill == null || et_skill.length() == 0) return;
                        mSkillAdapter.add(new Skill(et_skill.getText().toString()));
                        et_skill.setText("");
                        mSkillAdapter.notifyDataSetChanged();
                    }
                });
            }

            mJobsAdapter = EditableListAdapter.populateEditableList(this,
                    (ListView) findViewById(R.id.perfil_experiencia_laboral_list_editable),
                    fetchedUser.getWorkHistory()
            );
            ImageButton ib_workHistory =
                    (ImageButton) findViewById(R.id.boton_perfil_experiencia_laboral_agregar_item);
            if (ib_workHistory != null) {
                ib_workHistory.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        EditText et_employment = (EditText) findViewById(R.id.text_perfil_experiencia_laboral_new);
                        if (et_employment == null || et_employment.length() == 0) return;
                        mJobsAdapter.add(new Employment(et_employment.getText().toString()));
                        et_employment.setText("");
                        mJobsAdapter.notifyDataSetChanged();
                    }
                });
            }
        }

        // Togglear la visibilidad de views pertinentes a cada modo
        for (@IdRes int v : Utils.perfilVisibilityViews) {
            Utils.toggleViewVisibility(this, v);
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
        editedUser.setWorkHistory(mJobsAdapter.getList()); // TODO
        editedUser.setSkills(mSkillAdapter.getList());  // TODO

        fetchedUser = editedUser;
        return true;
    }


    public void refreshProfileInformation(final long idFetched) {

        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL()).buildUpon()
                .appendPath(getString(R.string.perfil_get_user_path))
                .appendPath(Long.toString(idFetched))
                .build();
        final String url = builtUri.toString();

        JsonObjectRequest jsObjRequest = new JsonObjectRequest
                (Request.Method.GET, url, null, new Response.Listener<JSONObject>() {

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

                }, new Response.ErrorListener() {

                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.d(LOG_TAG, "errortostring "+error.toString());
                        Log.d(LOG_TAG, "urrrrrrrrl: "+url);//
                        if (error instanceof ParseError && error.getCause() instanceof JSONException) {
                            Log.d(LOG_TAG, "JSONException! Intento refreshear de nuevo...");
                            refreshProfileInformation(idFetched);
                        }
                    }
                });
        jsObjRequest.setTag(LOG_TAG);

        RequestQueueSingleton.getInstance(this.getApplicationContext())
                .addToRequestQueue(jsObjRequest);
    }

    public void cargarFotoDePerfil(final long idFetched) {

        // TODO / DE PRUEBA
        if (mCurrentPhotoPath != null) {
            ImageView imageView = (ImageView) findViewById(R.id.perfil_image);
//            File file = new File(mCurrentPhotoPath);
            if (imageView != null) {// && file.exists()) {
                // Forma URI:
//                Uri imageContentUri = Uri.fromFile(file);
//                imageView.setImageURI(imageContentUri);
                // Forma Bitmap:
//                Bitmap imageBitmap = BitmapFactory.decodeFile(file.getAbsolutePath());
                try {
                    Bitmap imageBitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), mPhotoURI);
                    imageView.setImageBitmap(imageBitmap);
                    return;
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        }

        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL()).buildUpon()
                .appendPath(getString(R.string.perfil_get_photo_path))
                .appendPath(Long.toString(idFetched))
                .build();
        final String url = builtUri.toString();

        Utils.cargarImagenDeURLenImageView(getApplicationContext(),
                (ImageView) findViewById(R.id.perfil_image),
                url, LOG_TAG);
    }

    private void fillProfile(User mUser) {
        collapsingToolbarLayout.setTitle(mUser.getFullName());

        Utils.setTextViewText(this, R.id.text_perfil_mail, mUser.getEmail());
        Utils.setTextViewText(this, R.id.text_perfil_ciudad, mUser.getCity());
        Utils.setTextViewText(this, R.id.text_perfil_nacimiento, mUser.getLineaNacimiento());
        Utils.setTextViewText(this, R.id.text_perfil_resumen, mUser.getSummary(), true);
        Utils.setTextViewText(this, R.id.text_perfil_trabajo_actual, mUser.getTrabajosActuales(), true);

        populateContacts();

        Utils.populateStringList(this, R.id.perfil_experiencia_laboral_list, mUser.getListaJobs());
        Utils.populateStringList(this, R.id.perfil_skills_list, mUser.getListaSkills());
    }


    // TODO
    public void updateProfileInformation() {

        String username = "Robert";//

        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL()).buildUpon()
                .appendPath("user")
                .appendPath(username)
                .build();
        String url = builtUri.toString();

        final String mRequestBody = "Lottto";

        StringRequest mStringRequest = new StringRequest(Request.Method.POST,
                url, new Response.Listener<String>() {

            @Override
            public void onResponse(String response) {
                Log.i("VOLLEY", response);
            }

        }, new Response.ErrorListener() {

            @Override
            public void onErrorResponse(VolleyError error) {
                Log.e(LOG_TAG, "stringoferror "+error.toString());
                Toast.makeText(PerfilActivity.this, ":(", Toast.LENGTH_SHORT).show();
            }
        }) {

            @Override
            public byte[] getBody() throws AuthFailureError {
                try {
                    return mRequestBody == null ? null : mRequestBody.getBytes("utf-8");
                } catch (UnsupportedEncodingException uee) {
                    VolleyLog.wtf("Unsupported Encoding while trying to get the bytes of %s using %s",
                            mRequestBody, "utf-8");
                    return null;
                }
            }

//            @Override
//            protected Response<String> parseNetworkResponse(NetworkResponse response) {
//                String responseString = "";
//                if (response != null) {
//                    responseString = String.valueOf(response.statusCode);
//                    // can get more details such as response.headers
//                }
//                return Response.success(responseString, HttpHeaderParser.parseCacheHeaders(response));
//            }
        };

        mStringRequest.setTag(LOG_TAG);

        RequestQueueSingleton.getInstance(this.getApplicationContext())
                .addToRequestQueue(mStringRequest);
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
        File imageFile = new File(storageDir.getPath() + File.separator + imageFileName + ".jpg");

        mCurrentPhotoPath = imageFile.getAbsolutePath();//"file:" + imageFile.getAbsolutePath();//"content:" ?
        return imageFile;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == REQUEST_TAKE_PHOTO && resultCode == RESULT_OK) {

            /** Enviar la imagen al servidor como foto de perfil */ // TODO: EXTRAER
            File imageFile = new File(mCurrentPhotoPath);

            Uri builtUri = Uri.parse(Utils.getAppServerBaseURL()).buildUpon()
                    .appendPath(getString(R.string.perfil_post_photo_path))
                    .appendPath(Long.toString(fetchedUserID))
                    .build();
            final String url = builtUri.toString();

            if (imageFile.exists()) {
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


    private void populateContacts() {

//         TODO: if (contacts == 0) {
//            FrameLayout contactsFrameLayout = (FrameLayout) findViewById(R.id.perfil_contactos_frame);
//            if (contactsFrameLayout != null)
//                contactsFrameLayout.setVisibility(View.GONE);
//                return;
//        }

        GridView mGridView = (GridView) findViewById(R.id.perfil_contactos_list);
        if (mGridView != null) {

            final ContactCardAdapter mAdapter = new ContactCardAdapter(new ArrayList<User>());  //TODO hardcodeado
            mGridView.setAdapter(mAdapter);
            mGridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                    User clickedUser = mAdapter.getItem(position);
                    startActivity(
                            new Intent(PerfilActivity.this, PerfilActivity.class)
                                    .putExtra(FETCHED_USER_ID_MESSAGE, clickedUser.getId())
                    );
                }
            });

        } else {
            Log.e(LOG_TAG, "No se encontró el gridview de contactos!");
        }
    }

    private class ContactCardAdapter extends ArrayAdapter<User> {

        public ContactCardAdapter(List<User> userList) {
            super(PerfilActivity.this, R.layout.contact_card, userList);
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            View itemView = convertView;
            if (itemView == null) {
                itemView = LayoutInflater.from(getContext())
                        .inflate(R.layout.list_item_borrable, parent, false);
            }

            User user = getItem(position);
            if (user != null) {

                Uri builtUri = Uri.parse(Utils.getAppServerBaseURL()).buildUpon()
                        .appendPath(getString(R.string.perfil_get_photo_path))  // TODO: Thumbnail sale de otro lado??
                        .appendPath(Long.toString(user.getId()))
                        .build();
                Utils.cargarImagenDeURLenImageView(getApplicationContext(),
                        (ImageView) findViewById(R.id.contact_card_foto),
                        builtUri.toString(), LOG_TAG);

                TextView tv_nombre  = (TextView) itemView.findViewById(R.id.contact_card_nombre);
                if (tv_nombre != null)
                    tv_nombre.setText(user.getFullName());

                TextView tv_trabajo = (TextView) itemView.findViewById(R.id.contact_card_trabajo);
                if (tv_trabajo != null)
                    tv_trabajo.setText(user.getUltimoTrabajoActual());
            }

            return itemView;
        }
    }
}
