package ar.fiuba.jobify;

import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.res.ColorStateList;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Build;
import android.preference.PreferenceManager;
import android.support.annotation.ColorRes;
import android.support.annotation.IdRes;
import android.support.annotation.StringRes;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.VolleyLog;
import com.android.volley.toolbox.HttpHeaderParser;
import com.android.volley.toolbox.ImageRequest;
import com.android.volley.toolbox.JsonObjectRequest;

import org.apache.http.entity.ContentType;
import org.apache.http.entity.mime.HttpMultipartMode;
import org.apache.http.entity.mime.MultipartEntityBuilder;
import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * Created by martín on 29/09/16.
 * Wrapper para herramientas.
 */
public class Utils {

    private final static String LOG_TAG = Utils.class.getSimpleName();

    public static String getAppServerBaseURL(Context ctx) {
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(ctx);
        String ip = sharedPref.getString("pref_appServer_ip", ctx.getString(R.string.pref_default_appServer_ip));
        String puerto = sharedPref.getString("pref_appServer_puerto", ctx.getString(R.string.pref_default_appServer_puerto));

        return "http://" + ip + ":" + puerto + "/";
    }

    public static String getSharedServerBaseURL(Context ctx) {
        return ctx.getString(R.string.shared_server_base_url);
        /// TODO
//        return "";
    }

    public static void cargarImagenDeURLenImageView(final Context context, final ImageView imageView,
                                                    final String url, final String logTag) {
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
//                        Log.e(logTag, "Error de response, no pude cargar la imagen. (url: "+url+")");
                        if (error.networkResponse == null) return;
                        if (error.networkResponse.statusCode == 200) {
                            Log.e(logTag, "Problema con la imagen. Re-request");//
                            cargarImagenDeURLenImageView(context, imageView, url, logTag);
                        } else {
                            Log.e(logTag, "Error cargando imagen, response code: "
                                    +error.networkResponse.statusCode);
                        }
                    }
                }) ;
        RequestQueueSingleton.getInstance(context)
                .addToRequestQueue(request);
    }

    /**
     * Obtiene el JSON devuelto tras requestear un GET al AppServer a .../[getPath]/[idFetched]
     * En caso de éxito, se correrá responseListener.onResponse()
     */
    public static void getJsonFromAppServer(Context context, String getPathSegment, long idFetched,
                                            Response.Listener<JSONObject> responseListener,
                                            final String logTag) {

        getJsonFromAppServer(context, getPathSegment, idFetched, null, responseListener, logTag);
    }

    /**
     * Forma genérica que además permite enviar un parámetro JSON.
     */
    public static void getJsonFromAppServer(Context context, String getPathSegment, long idFetched,
                                            JSONObject jsonRequest,
                                            Response.Listener<JSONObject> responseListener,
                                            final String logTag) {

        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(context)).buildUpon()
                .appendPath(getPathSegment) // Podría generalizarlo haciendo un parámetro vectorizado
                .appendPath(Long.toString(idFetched))
                .build();
        final String url = builtUri.toString();

        getJsonFromUrl(context, url, jsonRequest, responseListener, logTag);
    }

    // TODO: Refactorizar mediante parámetro vectorizado
    public static void getJsonFromAppServer(Context context, String getPathSegment,
                                            Response.Listener<JSONObject> responseListener,
                                            final String logTag) {

        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(context)).buildUpon()
                .appendPath(getPathSegment) // Podría generalizarlo haciendo un parámetro vectorizado
                .build();
        final String url = builtUri.toString();

        getJsonFromUrl(context, url, null, responseListener, logTag);
    }

    public static void getJsonFromAppServer(Context ctx, String getPathSegment,
                                            JSONObject jsonRequest,
                                            Response.Listener<JSONObject> responseListener,
                                            Response.ErrorListener errorListener,
                                            final String logTag) {
        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(ctx)).buildUpon()
                .appendPath(getPathSegment) // Podría generalizarlo haciendo un parámetro vectorizado
                .build();
        final String url = builtUri.toString();

        fetchJsonFromUrl(ctx, Request.Method.GET, url, jsonRequest, responseListener,
                errorListener, logTag);
    }

    public static void getJsonFromSharedServer(Context context, String getPathSegment,
                                               Response.Listener<JSONObject> listener,
                                               final String logTag) {

        Uri builtUri = Uri.parse(Utils.getSharedServerBaseURL(context)).buildUpon()
                .appendPath(getPathSegment) // Podría generalizarlo haciendo un parámetro vectorizado
                .build();
        final String url = builtUri.toString();

        Log.d(LOG_TAG, "SS url: "+url);//

        getJsonFromUrl(context, url, null, listener, logTag);
    }

    public static void getJsonFromUrl(Context context, final String url, JSONObject jsonRequest,
                                      Response.Listener<JSONObject> responseListener,
                                      final String logTag) {
        fetchJsonFromUrl(context, Request.Method.GET, url, jsonRequest, responseListener, logTag);
    }

    public static void postJsonToAppServer(Context context, String getPathSegment, JSONObject jsonRequest,
                                     Response.Listener<JSONObject> responseListener,
                                           Response.ErrorListener errorListener,
                                     final String logTag) {

        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(context)).buildUpon()
                .appendPath(getPathSegment) // Podría generalizarlo haciendo un parámetro vectorizado
                .build();
        final String url = builtUri.toString();

        fetchJsonFromUrl(context, Request.Method.POST, url, jsonRequest, responseListener,
                errorListener, logTag);
    }

    public static void fetchJsonFromUrl(Context context, int method, final String url,
                                        JSONObject jsonRequest,
                                      Response.Listener<JSONObject> responseListener,
                                      final String logTag) {
        fetchJsonFromUrl(context, method, url, jsonRequest, responseListener,
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.d(logTag, "url: "+url);//
                        error.printStackTrace();
                    }
                }, logTag);
    }

    public static void fetchJsonFromUrl(Context context, int method, final String url,
                                        JSONObject jsonRequest,
                                        Response.Listener<JSONObject> responseListener,
                                        Response.ErrorListener errorListener,
                                        final String logTag) {
        JsonObjectRequest jsObjRequest = new JsonObjectRequest
                (method, url, jsonRequest, responseListener, errorListener);
        jsObjRequest.setTag(logTag);
        RequestQueueSingleton.getInstance(context).addToRequestQueue(jsObjRequest);
    }


    public static @IdRes int[] perfilVisibilityViews = {
            R.id.perfil_nombre_editable_frame, R.id.text_perfil_trabajo_actual,
            R.id.text_perfil_ciudad, R.id.text_perfil_ciudad_editable, R.id.boton_perfil_location,
            R.id.text_perfil_cant_recomendaciones, R.id.text_perfil_resumen,
            R.id.text_perfil_resumen_editable, R.id.perfil_experiencia_laboral_list,
            R.id.perfil_experiencia_laboral_list_editable, R.id.perfil_experiencia_laboral_list_new,
            R.id.perfil_skills_list, R.id.perfil_skills_list_editable, R.id.perfil_skills_list_new
    };

    public static String getTextViewText(AppCompatActivity activity, @IdRes int idRes) {
        String text = "";
        EditText et = (EditText) activity.findViewById(idRes);
        if (et != null) {
            text = et.getText().toString();
        }
        return text;
    }

    // Esconde TextView si text está vacío
    public static void setTextViewText(AppCompatActivity activity, @IdRes int idRes,
                                       CharSequence text) {
        setTextViewText(activity, idRes, text, false);
    }

    public static void setTextViewText(AppCompatActivity activity, @IdRes int idRes,
                                       CharSequence text, boolean hideEmptyText) {
        TextView tv = (TextView) activity.findViewById(idRes);
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

    public static void toggleViewVisibility(AppCompatActivity activity, @IdRes int idRes) {
        View v = activity.findViewById(idRes);
        if (v != null) {
            if (v.getVisibility() == View.GONE) {
                v.setVisibility(View.VISIBLE);
            } else {
                v.setVisibility(View.GONE);
            }
        }
    }

    public static void hideView(AppCompatActivity activity, @IdRes int idRes) {
        View v = activity.findViewById(idRes);
        if (v != null)
            v.setVisibility(View.GONE);
    }

    public static void editTextSetErrorAndFocus(AppCompatActivity activity, @IdRes int resId, String errorMessage) {
        EditText et = (EditText) activity.findViewById(resId);
        if (et != null) {
            et.setError(errorMessage);
            et.requestFocus();
        }
    }

    public static void populateStringList(AppCompatActivity activity, @IdRes int resId, List<String> list) {
        ListView mListView = (ListView) activity.findViewById(resId);
        if (mListView != null) {

            ArrayAdapter<String> mAdapter = new ArrayAdapter<>(
                    activity,
                    R.layout.list_item,
                    R.id.plain_text_list_item,
                    new ArrayList<String>()
            );
            mListView.setAdapter(mAdapter);

            mAdapter.addAll(list);
            mAdapter.notifyDataSetChanged();
            EditableListAdapter.actualizarAlturaDeListView(mListView, mAdapter);

        } else
            Log.e(LOG_TAG, "No se encontró el listview! resId: "+resId);
    }



    public static class PhotoMultipartRequest<T> extends Request<T> {

        private static final String FILE_PART_NAME = "file";

        private MultipartEntityBuilder mBuilder = MultipartEntityBuilder.create();
        private final Response.Listener<T> mListener;
        private final File mImageFile;
//        protected Map<String, String> headers;

        public PhotoMultipartRequest(String url, File imageFile, Response.Listener<T> listener,
                                     Response.ErrorListener errorListener) {
//            super(Method.POST, url, errorListener);
            super(Method.PUT, url, errorListener);

            mListener = listener;
            mImageFile = imageFile;

            buildMultipartEntity();
        }

        @Override
        public Map<String, String> getHeaders() throws AuthFailureError {
            Map<String, String> headers = super.getHeaders();

            if (headers == null
                    || headers.equals(Collections.emptyMap())) {
                headers = new HashMap<>();
            }

            headers.put("Accept", "application/json");

            return headers;
        }

        private void buildMultipartEntity() {
            mBuilder.addBinaryBody(FILE_PART_NAME, mImageFile, ContentType.create("image/jpeg"), mImageFile.getName());
            mBuilder.setMode(HttpMultipartMode.BROWSER_COMPATIBLE);
            mBuilder.setLaxMode().setBoundary("xx").setCharset(Charset.forName("UTF-8"));
        }

        @Override
        public String getBodyContentType() {
            return mBuilder.build().getContentType().getValue(); //contentTypeHeader
        }

        @Override
        public byte[] getBody() throws AuthFailureError {
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            try {
                mBuilder.build().writeTo(bos);
            } catch (IOException e) {
                VolleyLog.e("IOException writing to ByteArrayOutputStream bos, building the multipart request.");
            }

            return bos.toByteArray();
        }

        @Override
        protected Response<T> parseNetworkResponse(NetworkResponse response) {
            return Response.success(null, HttpHeaderParser.parseCacheHeaders(response));
        }

        @Override
        protected void deliverResponse(T response) {
            mListener.onResponse(response);
        }
    }



    public static void borrarSharedServerData(Activity activity, String className) {
        SharedPreferences mPrefs = activity.getPreferences(Activity.MODE_PRIVATE);
        SharedPreferences.Editor prefsEditor = mPrefs.edit();
        prefsEditor.remove(className);
        prefsEditor.apply();
    }

    /**
     * Busca los datos primero en las preferencias y a falta de ello la fetchea.
     * @param className Se usará para buscar los datos en las preferencias.
     */
    public static String getSharedServerDataJsonString(Activity activity,
                                                  final String className, @StringRes int SsGetPath) {
        SharedPreferences mPrefs = activity.getPreferences(Activity.MODE_PRIVATE);
        final SharedPreferences.Editor prefsEditor = mPrefs.edit();
        String json = mPrefs.getString(className, "");

        if (json.equals("")) {
            getJsonFromSharedServer(activity, activity.getString(SsGetPath),
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            prefsEditor.putString(className, response.toString());
                            prefsEditor.apply();
                            Log.d(LOG_TAG, "Fetcheé "+className);
                        }
                    }, LOG_TAG);
            return null;
        }

        return json;
    }

    @SuppressWarnings("deprecation")
    public static ColorStateList getColorStateList(Context context, @ColorRes int id) {

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            return context.getResources().getColorStateList(id, context.getTheme());
        } else {
            return context.getResources().getColorStateList(id);
        }
    }

    public static void confirmarAccion(Context context, String title, String message,
                                       DialogInterface.OnClickListener listener) {
        new AlertDialog.Builder(context)
                .setTitle(title)
                .setMessage(message)
                .setIcon(android.R.drawable.ic_dialog_alert)
                .setPositiveButton(android.R.string.yes, listener)
                .setNegativeButton(android.R.string.no, null)
                .show();
    }
}
