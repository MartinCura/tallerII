package ar.fiuba.jobify;

import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Bundle;
import android.preference.MultiSelectListPreference;
import android.preference.PreferenceManager;
import android.support.annotation.LayoutRes;
import android.support.design.widget.CollapsingToolbarLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
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
import org.w3c.dom.Text;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import ar.fiuba.jobify.app_server_api.User;

public class PerfilActivity extends NavDrawerActivity {

    private final String LOG_TAG = PerfilActivity.class.getSimpleName();

//    private TextView textoEjemplo;//
    private CollapsingToolbarLayout collapsingToolbarLayout;//

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_perfil_drawer);

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

        FloatingActionButton fabChatearOEditar = (FloatingActionButton) findViewById(R.id.fab_chatear_o_editar);
        if (fabChatearOEditar != null) {
            fabChatearOEditar.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    // TODO: if (perfil propio) Editar, else Chatear
                }
            });
        }

        collapsingToolbarLayout.setTitle(getString(R.string.perfil_nombre_y_apellido_default));

//        textoEjemplo = (TextView) findViewById(R.id.textview_perfil_nombre_y_apellido);

        //Toast.makeText(this, "PerfilActivity onCreate", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void setContentView(@LayoutRes int layoutResID) {
        super.setContentView(layoutResID);
        onCreateDrawer();
    }

    @Override
    protected void onResume() {
        super.onResume();

        refreshProfileInformation(1);   // TODO hardcodeo
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

        String baseURL = "http://" + ip + ":" + puerto + "/";
        return baseURL;
    }

    public void refreshProfileInformation(final int idFetched) {

        Uri builtUri = Uri.parse(getAppServerBaseURL()).buildUpon()
                .appendPath(getString(R.string.perfil_get_user_path))
                .appendPath(Integer.toString(idFetched))
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

    private void fillProfile(User mUser) {
        collapsingToolbarLayout.setTitle(mUser.getFullName());

        setTextViewText(R.id.text_perfil_mail, mUser.getEmail());
        setTextViewText(R.id.text_perfil_ciudad, mUser.getCity());
        setTextViewText(R.id.text_perfil_nacimiento, mUser.getLineaNacimiento());
        setTextViewText(R.id.text_perfil_resumen, mUser.getSummary());
        setTextViewText(R.id.text_perfil_trabajo_actual, mUser.getTrabajoActual());

        populateStringList(R.id.perfil_experiencia_laboral_list, mUser.getListaJobs());
        populateStringList(R.id.perfil_skills_list, mUser.getListaSkills());
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

            Log.d(LOG_TAG, "TAMA;O: "+list.size());//
            mAdapter.addAll(list);
            mAdapter.notifyDataSetChanged();

        } else
            Log.e(LOG_TAG, "No se encontró el listview! idRes: "+idRes);
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
//                textoEjemplo.setText("[No se obtuvo nada de la URL hardcodeada.]");
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
}
