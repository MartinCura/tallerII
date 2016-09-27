package ar.fiuba.jobify;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.annotation.LayoutRes;
import android.support.design.widget.CollapsingToolbarLayout;
import android.support.design.widget.FloatingActionButton;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
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

import ar.fiuba.jobify.app_server_api.User;

public class PerfilActivity extends NavDrawerActivity {

    private final String LOG_TAG = PerfilActivity.class.getSimpleName();

    public final static String FETCHED_USER_ID_MESSAGE = "ar.fiuba.jobify.FETCHED_USER_ID_MESSAGE";
    private long fetchedUserID = 2;

    private CollapsingToolbarLayout collapsingToolbarLayout;//

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
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
                    // TODO: Editar
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


    private String getAppServerBaseURL() {
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(this);
        String ip = sharedPref.getString("pref_appServer_ip", getString(R.string.pref_default_appServer_ip));
        String puerto = sharedPref.getString("pref_appServer_puerto", getString(R.string.pref_default_appServer_puerto));

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

                        if (mUser != null)
                            fillProfile(mUser);

                        else {
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
        final String url = builtUri.toString(); //"http://i.imgur.com/7spzG.png";

        final ImageView imageView = (ImageView) findViewById(R.id.perfil_image);
        if (imageView == null) {
            Log.e(LOG_TAG, "No pude encontrar el ImageView, no cargo foto de perfil.");
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
                        Log.e(LOG_TAG, "Error de response, no pude cargar la foto de perfil.");
                        if (error.networkResponse == null) return;
                        Log.e(LOG_TAG, "Network response status code: "+error.networkResponse.statusCode);
                        if (error.networkResponse.statusCode == 200) {
                            Log.e(LOG_TAG, "Reintento cargar foto de perfil");//
                            cargarFotoDePerfil(idFetched);
                        }
                    }
                }) ;
        RequestQueueSingleton.getInstance(this.getApplicationContext())
                .addToRequestQueue(request);
    }


    private void fillProfile(User mUser) {
        collapsingToolbarLayout.setTitle(mUser.getFullName());

        setTextViewText(R.id.text_perfil_mail, mUser.getEmail());
        setTextViewText(R.id.text_perfil_ciudad, mUser.getCity());
        setTextViewText(R.id.text_perfil_nacimiento, mUser.getLineaNacimiento());
        setTextViewText(R.id.text_perfil_resumen, mUser.getSummary());
        setTextViewText(R.id.text_perfil_trabajo_actual, mUser.getTrabajosActuales());

        populateStringList(R.id.perfil_experiencia_laboral_list, mUser.getListaJobs());
        populateStringList(R.id.perfil_skills_list, mUser.getListaSkills());

        populateContacts();
    }

    // Esconde TextView si text está vacío
    public void setTextViewText(int idRes, CharSequence text) {
        TextView tv = (TextView) findViewById(idRes);
        if (tv != null) {
            tv.setText(text);

            if (text.equals(""))
                tv.setVisibility(View.GONE);
            else
                tv.setVisibility(View.VISIBLE);

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

            int totalHeight = 0;
            for (int i = 0; i < mAdapter.getCount(); i++) {
                View listItem = mAdapter.getView(i, null, mListView);
                listItem.measure(0, 0);
                totalHeight += listItem.getMeasuredHeight();
            }

            // Para asegurarse de que la altura esté bien.
            ViewGroup.LayoutParams params = mListView.getLayoutParams();
            params.height = totalHeight +
                    (mListView.getDividerHeight() * (mAdapter.getCount() - 1));
            mListView.setLayoutParams(params);

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

    private class EditableListAdapter extends ArrayAdapter<String> {

        public EditableListAdapter(List<String> list) {
            super(PerfilActivity.this, R.layout.list_item_borrable, list);
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            View itemView = convertView;
            if (itemView == null) {
                itemView = LayoutInflater.from(getContext())
                                .inflate(R.layout.list_item_borrable, parent, false);
            }

            final String itemString = getItem(position);
            if (itemString != null) {

                EditText item_tv = (EditText) itemView.findViewById(R.id.text_list_item_editable);
                if (item_tv != null)
                    item_tv.setText(itemString);

                ImageButton botonRemove = (ImageButton) itemView.findViewById(R.id.boton_borrar_item);
                if (botonRemove != null) {
                    botonRemove.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            remove(itemString);     // TODO: Revisar funcionamiento.
                            notifyDataSetChanged();
                        }
                    });
                }
            }

            return itemView;
        }
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

                ImageView tv_thumbnail = (ImageView) itemView.findViewById(R.id.contact_card_foto);
                if (tv_thumbnail != null) {
                    // TODO: Cargar foto a partir de URL
                }

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
