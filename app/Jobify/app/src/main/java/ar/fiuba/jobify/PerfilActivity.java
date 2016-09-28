package ar.fiuba.jobify;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.ColorStateList;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.annotation.IdRes;
import android.support.annotation.LayoutRes;
import android.support.design.widget.CollapsingToolbarLayout;
import android.support.design.widget.FloatingActionButton;
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
import com.android.volley.toolbox.ImageRequest;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;

import org.json.JSONException;
import org.json.JSONObject;

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

    // TODO: nacimiento, foto
    private void switchEditMode() {
        FloatingActionButton fabEditar = (FloatingActionButton) findViewById(R.id.fab_editar);

        if (inEditingMode) {    /** Modo normal */
            // Modificar usuario con contenido de los campos; si hay errores de input cancelar
            if (!capturarInputPerfilUsuario()) {
                Toast.makeText(this, "Usuario no modificado", Toast.LENGTH_LONG)
                        .show();
                return;
            }

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

        } else {    /** Modo edición */
            // Cambia color del botón de edición
            if (fabEditar != null) {
                ColorStateList csl = new ColorStateList(
                        new int[][] { new int[] { android.R.attr.state_enabled } },
                        new int[] { Color.GREEN });
//                fabEditar.setBackgroundTintList(csl);
                fabEditar.setBackgroundColor(Color.BLUE);
            }

            // Cambia funcionamiento del "volver" TODO

            // Precargar campos con valores actuales
            setTextViewText(R.id.text_perfil_nombre_editable, fetchedUser.getFirstName());
            setTextViewText(R.id.text_perfil_apellido_editable, fetchedUser.getLastName());
            setTextViewText(R.id.text_perfil_ciudad_editable, fetchedUser.getCity());
            setTextViewText(R.id.text_perfil_resumen_editable, fetchedUser.getSummary());

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
        @IdRes int[] visibilityViews = {
                R.id.perfil_toolbar,
                R.id.perfil_nombre_editable_frame,
                R.id.text_perfil_trabajo_actual,
                R.id.text_perfil_ciudad,
                R.id.text_perfil_ciudad_editable,
                R.id.boton_perfil_location,
                R.id.text_perfil_cant_recomendaciones,
                R.id.text_perfil_resumen,
                R.id.text_perfil_resumen_editable,
                R.id.perfil_contactos_frame,
                R.id.perfil_experiencia_laboral_list,
                R.id.perfil_experiencia_laboral_list_editable,
                R.id.perfil_experiencia_laboral_list_new,
                R.id.perfil_skills_list,
                R.id.perfil_skills_list_editable,
                R.id.perfil_skills_list_new
        };
        for (@IdRes int v : visibilityViews) {
            toggleViewVisibility(v);
        }

        inEditingMode = !inEditingMode;
    }

    private boolean capturarInputPerfilUsuario() {
        User editedUser = new User(fetchedUser);

        // Si el input de algún campo es incorrecto, falla antes modificar nada.
        if (!editedUser.setFirstName(getTextViewText(R.id.text_perfil_nombre_editable))) {
            EditText et_nombre = (EditText) findViewById(R.id.text_perfil_nombre_editable);
            if (et_nombre != null) {
                et_nombre.setError("Nombre muy corto o muy largo");
                et_nombre.requestFocus();
            }
            return false;
        }
        if (!editedUser.setLastName(getTextViewText(R.id.text_perfil_apellido_editable))) {
            EditText et_apellido = (EditText) findViewById(R.id.text_perfil_apellido_editable);
            if (et_apellido != null) {
                et_apellido.setError("Apellido muy corto o muy largo");
                et_apellido.requestFocus();
            }
            return false;
        }
        if (!editedUser.setCity(getTextViewText(R.id.text_perfil_ciudad_editable))) {
            EditText et_ciudad = (EditText) findViewById(R.id.text_perfil_ciudad_editable);
            if (et_ciudad != null) {
                et_ciudad.setError("!");
                et_ciudad.requestFocus();
            }
            return false;
        }
        if (!editedUser.setSummary(getTextViewText(R.id.text_perfil_resumen_editable))) {
            EditText et_resumen = (EditText) findViewById(R.id.text_perfil_resumen_editable);
            if (et_resumen != null) {
                et_resumen.setError("Máximo 1000 caracteres");  // hardcodeado!
                et_resumen.requestFocus();
            }
            return false;
        }
        editedUser.setWorkHistory(mJobsAdapter.getList()); // TODO
        editedUser.setSkills(mSkillAdapter.getList());  // TODO

        fetchedUser = editedUser;
        return true;
    }

    private String getTextViewText(@IdRes int idRes) {
        String text = "";
        EditText et = (EditText) findViewById(idRes);
        if (et != null) {
            text = et.getText().toString();
        }
        return text;
    }

    private void toggleViewVisibility(@IdRes int idRes) {
        View v = findViewById(idRes);
        if (v != null) {
            if (v.getVisibility() == View.GONE) {
                v.setVisibility(View.VISIBLE);
            } else {
                v.setVisibility(View.GONE);
            }
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

    public static String getAppServerBaseURL() {
        Context c = getContext();
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(c);
        String ip = sharedPref.getString("pref_appServer_ip", c.getString(R.string.pref_default_appServer_ip));
        String puerto = sharedPref.getString("pref_appServer_puerto", c.getString(R.string.pref_default_appServer_puerto));

        return "http://" + ip + ":" + puerto + "/";
    }

    public void refreshProfileInformation(final long idFetched) {

        Uri builtUri = Uri.parse(getAppServerBaseURL()).buildUpon()
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
//                        error.printStackTrace();//
                        Log.d(LOG_TAG, "errortostring "+error.toString());
                        Log.d(LOG_TAG, "urrrrrrrrl: "+url);//

//                        textView.setText("[No se obtuvo nada de la URL hardcodeada.]");
//                        Toast.makeText(getActivity(), ":(", Toast.LENGTH_SHORT).show();
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

        Uri builtUri = Uri.parse(getAppServerBaseURL()).buildUpon()
                .appendPath(getString(R.string.perfil_get_photo_path))
                .appendPath(Long.toString(idFetched))
                .build();
        final String url = builtUri.toString();

        cargarImagenDeURLenImageView(getApplicationContext(),
                (ImageView) findViewById(R.id.perfil_image),
                url, LOG_TAG);
    }

    public static void cargarImagenDeURLenImageView(final Context context, final ImageView imageView, final String url, final String logTag) {

        if (imageView == null) {
            Log.e(logTag, "No pude encontrar el ImageView, no cargo imagen. ("+url+")");
            return;
        }

        ImageRequest request = new ImageRequest(url,
                new Response.Listener<Bitmap>() {
                    @Override
                    public void onResponse(Bitmap bitmap) {
                        imageView.setImageBitmap(bitmap);

                    }
                }, imageView.getWidth(), imageView.getHeight(),
                ImageView.ScaleType.CENTER_INSIDE, null,
                new Response.ErrorListener() {
                    public void onErrorResponse(VolleyError error) {
                        Log.e(logTag, "Error de response, no pude cargar la imagen.");
                        if (error.networkResponse == null) return;
                        Log.e(logTag, "Network response status code: "+error.networkResponse.statusCode);
                        if (error.networkResponse.statusCode == 200) {
                            Log.e(logTag, "Problema con la imagen. Re-request");//
                            cargarImagenDeURLenImageView(context, imageView, url, logTag);
                        }
                    }
                }) ;
        RequestQueueSingleton.getInstance(context)
                .addToRequestQueue(request);
    }

    private void fillProfile(User mUser) {
        collapsingToolbarLayout.setTitle(mUser.getFullName());

        setTextViewText(R.id.text_perfil_mail, mUser.getEmail());
        setTextViewText(R.id.text_perfil_ciudad, mUser.getCity());
        setTextViewText(R.id.text_perfil_nacimiento, mUser.getLineaNacimiento());
        setTextViewText(R.id.text_perfil_resumen, mUser.getSummary(), true);
        setTextViewText(R.id.text_perfil_trabajo_actual, mUser.getTrabajosActuales(), true);

        populateStringList(R.id.perfil_experiencia_laboral_list, mUser.getListaJobs());
        populateStringList(R.id.perfil_skills_list, mUser.getListaSkills());

        populateContacts();
    }

    // Esconde TextView si text está vacío
    public void setTextViewText(@IdRes int idRes, CharSequence text) {
        setTextViewText(idRes, text, false);
    }

    public void setTextViewText(@IdRes int idRes, CharSequence text, boolean hideEmptyText) {
        TextView tv = (TextView) findViewById(idRes);
        if (tv != null) {
            tv.setText(text);

            if (hideEmptyText) {
                if (text.equals(""))
                    tv.setVisibility(View.GONE);
                else
                    tv.setVisibility(View.VISIBLE);
            }

        } else
            Log.e(LOG_TAG, "No se encontró el textview! idRes: "+idRes+", text: "+text);
    }

    public void populateStringList(int idRes, List<String> list) {
        ListView mListView = (ListView) findViewById(idRes);
        if (mListView != null) {

            ArrayAdapter<String> mAdapter = new ArrayAdapter<>(
                    this,
                    R.layout.list_item,
                    R.id.plain_text_list_item,
                    new ArrayList<String>()
            );
            mListView.setAdapter(mAdapter);

            mAdapter.addAll(list);
            mAdapter.notifyDataSetChanged();
            EditableListAdapter.actualizarAlturaDeListView(mListView, mAdapter);

        } else
            Log.e(LOG_TAG, "No se encontró el listview! idRes: "+idRes);
    }

    private void populateContacts() {

//         TODO: if (contacts == 0) {
//            FrameLayout contactsFrameLayout = (FrameLayout) findViewById(R.id.perfil_contactos_frame);
//            if (contactsFrameLayout != null)
//                contactsFrameLayout.setVisibility(View.GONE);
//        } else {

        GridView mGridView = (GridView) findViewById(R.id.perfil_contactos_list);
        if (mGridView != null) {

            final CardAdapter mAdapter = new CardAdapter(new ArrayList<User>());  //TODO hardcodeado
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

//        }
    }


    public void updateProfileInformation() {

        String username = "Robert";//

        Uri builtUri = Uri.parse(getAppServerBaseURL()).buildUpon()
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




    private class CardAdapter extends ArrayAdapter<User> {

        public CardAdapter(List<User> userList) {
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

                Uri builtUri = Uri.parse(getAppServerBaseURL()).buildUpon()
                        .appendPath(getString(R.string.perfil_get_photo_path))  // TODO: Thumbnail sale de otro lado??
                        .appendPath(Long.toString(user.getId()))
                        .build();
                cargarImagenDeURLenImageView(getApplicationContext(),
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
