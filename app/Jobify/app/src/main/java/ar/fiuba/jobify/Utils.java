package ar.fiuba.jobify;

import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
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
import org.json.JSONException;
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

    public static void iniciarPerfilActivity(Activity callingActivity, long fetchedUserId,
                                             boolean comenzarEnModoEdicion) {
        callingActivity.startActivity(
                new Intent(callingActivity, PerfilActivity.class)
                        .putExtra(PerfilActivity.FETCHED_USER_ID_MESSAGE, fetchedUserId)
                        .putExtra(PerfilActivity.PERFIL_MODE_MESSAGE, comenzarEnModoEdicion)
        );
    }

    public static void iniciarConversacionActivity(Activity callingActivity, long remitenteUserId) {
        callingActivity.startActivity(
                new Intent(callingActivity, ConversacionActivity.class)
                        .putExtra(ConversacionActivity.CORRESPONSAL_ID_MESSAGE, remitenteUserId)
        );
    }

    /////////////////////////////////////////// FETCHER ////////////////////////////////////////////

    public static String getAppServerBaseURL(Context ctx) {
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(ctx);
        String ip = sharedPref.getString("pref_appServer_ip", ctx.getString(R.string.pref_default_appServer_ip));
        String puerto = sharedPref.getString("pref_appServer_puerto", ctx.getString(R.string.pref_default_appServer_puerto));

        try {
            if (Integer.parseInt(puerto) <= 0)
                return "http://" + ip + "/";
        } catch (NumberFormatException ex) {
            Log.e(LOG_TAG, "Puerto no es un número");
        }

        return "http://" + ip + ":" + puerto + "/";
    }

    public static String getSharedServerBaseURL(Context ctx) {
        return ctx.getString(R.string.shared_server_base_url); // hardcodeado
    }

    /**
     * Devuelve un String con la URL para utilizar en un fetch, en la forma:
     * {@code http://<AppServer>/<path>/<idFetched>?<queries_y_valores>}
     */
    public static String getAppServerUrl(Context ctx, String path, long idFetched,
                                         HashMap<String, String> map) {
        Uri.Builder builder = Uri.parse(Utils.getAppServerBaseURL(ctx)).buildUpon()
                .appendPath(path)
                .appendPath(Long.toString(idFetched));
        for (Map.Entry<String, String> entry : map.entrySet())
            builder.appendQueryParameter(entry.getKey(), entry.getValue());
        return builder.build().toString();
    }

    /**
     * Devuelve un String con la URL para utilizar en un fetch, en la forma:
     * {@code http://<AppServer>/<path>?<queries_y_valores>}
     */
    public static String getAppServerUrl(Context ctx, String path, HashMap<String, String> map) {
        Uri.Builder builder = Uri.parse(Utils.getAppServerBaseURL(ctx)).buildUpon()
                .appendPath(path);
//                .appendPath(Long.toString(idFetched));
        for (Map.Entry<String, String> entry : map.entrySet())
            builder.appendQueryParameter(entry.getKey(), entry.getValue());
        return builder.build().toString();
    }

    /**
     * Devuelve un String con la URL para utilizar en un fetch, en la forma:
     * {@code http://<AppServer>/<paths/separados/por/barras>/<idFetched>}
     */
    public static String getAppServerUrl(Context ctx, long idFetched, String ... paths) {
        Uri.Builder builder = Uri.parse(Utils.getAppServerBaseURL(ctx)).buildUpon();
        for (String path : paths)
            builder = builder.appendPath(path);
        Uri builtUri = builder.appendPath(Long.toString(idFetched)).build();
        return builtUri.toString();
    }

    /**
     * Devuelve un String con la URL para utilizar en un fetch, en la forma:
     * {@code http://<AppServer>/<paths/separados/por/barras>}
     */
    public static String getAppServerUrl(Context ctx, String ... paths) {
        Uri.Builder builder = Uri.parse(Utils.getAppServerBaseURL(ctx)).buildUpon();
        for (String path : paths)
            builder = builder.appendPath(path);
        Uri builtUri = builder.build();
        return builtUri.toString();
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

        getJsonFromUrl(context, getAppServerUrl(context, idFetched, getPathSegment), jsonRequest,
                responseListener, logTag);
    }

    public static void getJsonFromAppServer(Context context, String getPathSegment,
                                            Response.Listener<JSONObject> responseListener,
                                            final String logTag) {

        getJsonFromUrl(context, getAppServerUrl(context, getPathSegment), null, responseListener,
                logTag);
    }

//    public static void getJsonFromAppServer(Context ctx, String getPathSegment,
//                                            JSONObject jsonRequest,
//                                            Response.Listener<JSONObject> responseListener,
//                                            Response.ErrorListener errorListener,
//                                            final String logTag) {
//
//        fetchJsonFromUrl(ctx, Request.Method.GET, getAppServerUrl(ctx, getPathSegment), jsonRequest,
//                responseListener, errorListener, logTag);
//    }

    // TEMPORAL, si es cambiado por un acceso al AppServer
    public static void getJsonFromSharedServer(Context context, String getPathSegment,
                                               Response.Listener<JSONObject> listener,
                                               final String logTag) {

        Uri builtUri = Uri.parse(Utils.getSharedServerBaseURL(context)).buildUpon()
                .appendPath(getPathSegment) // Podría generalizarlo haciendo un parámetro vectorizado
                .build();
        final String url = builtUri.toString();
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

        fetchJsonFromUrl(context, Request.Method.POST, getAppServerUrl(context, getPathSegment),
                jsonRequest, responseListener, errorListener, logTag);
    }

    public static void putJsonToAppServer(Context context, String getPathSegment, JSONObject jsonRequest,
                                           Response.Listener<JSONObject> responseListener,
                                           final String logTag) {

        fetchJsonFromUrl(context, Request.Method.PUT, getAppServerUrl(context, getPathSegment),
                jsonRequest, responseListener, logTag);
    }

    public static void fetchJsonFromUrl(Context context, int method, final String url,
                                        JSONObject jsonRequest,
                                        final Response.Listener<JSONObject> responseListener,
                                        final String logTag) {

        fetchJsonFromUrl(context, method, url, jsonRequest, responseListener,
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {

                        // En caso de que no debía recibirse nada y el retorno fue correcto,
                        // correr el método para response correcto.
                        // // No debería correrse nunca ahora que modifiqué JsonObjectRequest
                        if (error.networkResponse != null && error.networkResponse.statusCode == 200) {
                            try {
                                responseListener.onResponse(new JSONObject("{}"));
                            } catch (JSONException ex) {
                                Log.e(LOG_TAG, "JSON vacío no aceptado");
                            }
                        } else {
                            Log.d(logTag, "Error Listener. URL: " + url);
                            if (error.networkResponse != null) {
                                if (error.networkResponse.statusCode == 403)
                                    Log.d(logTag, error.networkResponse.statusCode + " FORBIDDEN");
                                else
                                    Log.d(logTag, "Status code: " + error.networkResponse.statusCode);
                            }
                            error.printStackTrace();
                        }
                    }
                }, logTag);
    }

    /**
     * Fetchea un json. Los demás métodos overloadean este.
     */
    public static void fetchJsonFromUrl(final Context context, int method, final String url,
                                        JSONObject jsonRequest,
                                        Response.Listener<JSONObject> responseListener,
                                        Response.ErrorListener errorListener, final String logTag) {

        JsonObjectRequest jsObjRequest = new JsonObjectRequest
                (method, url, jsonRequest, responseListener, errorListener) {

                    @Override
                    public Map<String, String> getHeaders() throws AuthFailureError {
                        Map<String, String> headers = new HashMap<>();
//                        headers.put("Accept", "application/json");
//                        headers.put("Content-Type", "application/json");//; charset=utf-8");
//                        headers.put("accept-encoding", "gzip, deflate");
//                        headers.put("accept-language", "en-US,en;q=0.8");
//                        headers.put("user-agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/" +
//                                "537.36 (KHTML, like Gecko) Chrome/54.0.2840.71 Safari/537.36");
                        headers.put("Connection", "close");//Te amo, header que soluciona cosas ~ mc
                        headers.put("Authorization", getToken(context));
                        Log.d(LOG_TAG, "Getting token: "+getToken(context));//
                        return headers;
                    }

                    @Override
                    public String getBodyContentType() {
                        return "application/json";
                    }
        };
        jsObjRequest.setTag(logTag);
        RequestQueueSingleton.getInstance(context).addToRequestQueue(jsObjRequest);
    }


    public static void cargarImagenDeURLenImageView(final Context ctx, final ImageView imageView,
                                                    final String  url, final String logTag) {
        cargarImagenDeURLenImageView(ctx, imageView, url, logTag, false);
    }

    public static void cargarImagenDeURLenImageView(final Context ctx, final ImageView imageView,
                                  final String url, final String logTag, final boolean squareCrop) {

        if (imageView == null) {
            //Log.e(logTag, "No pude encontrar el ImageView, no cargo imagen. ("+url+")");
            return;
        }
        ImageRequest request = new ImageRequest(url,
                new Response.Listener<Bitmap>() {
                    @Override
                    public void onResponse(Bitmap bitmap) {
                        if (squareCrop)
                            imageView.setImageBitmap(cropToSquare(bitmap));
                        else
                            imageView.setImageBitmap(bitmap);

                    }

                }, imageView.getWidth(), imageView.getHeight(),
                ImageView.ScaleType.CENTER_INSIDE, null,
                new Response.ErrorListener() {
                    public void onErrorResponse(VolleyError error) {
                        if (error.networkResponse == null) {
                            Log.e(logTag, "Error de response, no pude cargar la imagen." +
                                    "(url: "+url+")");
                            return;
                        }
                        //error.printStackTrace();//
                        if (error.networkResponse.statusCode == 200) {
                            Log.e(logTag, "Problema con la imagen. Re-request");//
                            cargarImagenDeURLenImageView(ctx, imageView, url, logTag);
                        } else if (error.networkResponse.statusCode == 403) {
                            Log.d(logTag, error.networkResponse.statusCode + " FORBIDDEN");
                        } else {
                            Log.e(logTag, "Error cargando imagen, response code: "
                                    +error.networkResponse.statusCode);
                        }
                    }
                }) {
                    @Override
                    public Map<String, String> getHeaders() throws AuthFailureError {
                        Map<String, String> headers = new HashMap<>();
                        headers.put("Authorization", getToken(ctx));
                        Log.d(LOG_TAG, "Getting token: "+getToken(ctx));//
                        return headers;
                    }
        };
        RequestQueueSingleton.getInstance(ctx)
                .addToRequestQueue(request);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////

    public static @IdRes int[] perfilVisibilityViews = {
            R.id.perfil_nombre_editable_frame, R.id.text_perfil_trabajo_actual,
            R.id.text_perfil_ciudad, R.id.text_perfil_ciudad_editable, R.id.boton_perfil_location,
            R.id.text_perfil_cant_recomendaciones, R.id.text_perfil_resumen,
            R.id.text_perfil_nacimiento, R.id.perfil_nacimiento_editable,
            R.id.text_perfil_resumen_editable_wrapper, R.id.perfil_experiencia_laboral_list,
            R.id.perfil_experiencia_laboral_list_editable, R.id.perfil_experiencia_laboral_list_new,
            R.id.perfil_skills_list, R.id.perfil_skills_list_editable, R.id.perfil_skills_list_new
    };

    // Devuelve -1 en caso de error
    public static int getTextViewInt(AppCompatActivity activity, @IdRes int idRes) {
        int ret = -1;
        try {
            ret = Integer.parseInt(getTextViewText(activity, idRes));
        } catch (NumberFormatException ex) {
            Log.i(LOG_TAG, "No se pudo parsear un número");
        }
        return ret;
    }

    // Devuelve string vacío en caso de error
    public static String getTextViewText(AppCompatActivity activity, @IdRes int idRes) {
        String text = "";
        EditText et = (EditText) activity.findViewById(idRes);
//        TextView et = (TextView) activity.findViewById(idRes);
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
                if (text.length() == 0)
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

    public static void showView(AppCompatActivity activity, @IdRes int idRes) {
        View v = activity.findViewById(idRes);
        if (v != null)
            v.setVisibility(View.VISIBLE);
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
        private final Context ctx;
//        protected Map<String, String> headers;

        public PhotoMultipartRequest(Context context, String url, File imageFile,
                                     Response.Listener<T> listener,
                                     Response.ErrorListener errorListener) throws IOException {
//            super(Method.POST, url, errorListener);
            super(Method.PUT, url, errorListener);

            this.ctx = context;
            mListener = listener;
            mImageFile = imageFile;

            buildMultipartEntity();
        }

        @Override
        public Map<String, String> getHeaders() throws AuthFailureError {
            Map<String, String> headers = super.getHeaders();
            if (headers == null || headers.equals(Collections.emptyMap())) {
                headers = new HashMap<>();
            }
            headers.put("Accept", "application/json");
            headers.put("Authorization", getToken(ctx));
            Log.d(LOG_TAG, "Getting token: "+getToken(ctx));//
            return headers;
        }

        private void buildMultipartEntity() throws IOException {
            mBuilder.addBinaryBody(FILE_PART_NAME, mImageFile, ContentType.create("image/jpeg"),
                    mImageFile.getName());
            mBuilder.setMode(HttpMultipartMode.BROWSER_COMPATIBLE);
            mBuilder.setLaxMode().setBoundary("xx").setCharset(Charset.forName("UTF-8"));

//            byte[] fileData = new byte[(int) mImageFile.length()];
//            DataInputStream dis = new DataInputStream(new FileInputStream(mImageFile));
//            dis.readFully(fileData);
//            dis.close();

//            FileBody fileBody = new FileBody(mImageFile);
//            mBuilder.addPart(FILE_PART_NAME, fileBody);

//            mBuilder.addBinaryBody(FILE_PART_NAME, fileData);
//            mBuilder.setLaxMode();
//            mBuilder.setBoundary("");
//            mBuilder.setCharset(Charset.forName("UTF-8"));
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
                VolleyLog.e("IOException writing to ByteArrayOutputStream bos, " +
                            "building the multipart request.");
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

    private static String getToken(Context ctx) {
        SharedPreferences sharedPref =
                ctx.getSharedPreferences(ctx.getString(R.string.shared_pref_connected_user), 0);
        return sharedPref.getString(ctx.getString(R.string.stored_connected_user_token), "null");
    }

    public static Bitmap cropToSquare(Bitmap bitmap) {
        int width  = bitmap.getWidth();
        int height = bitmap.getHeight();
        int newWidth = (height > width) ? width : height;
        int newHeight = (height > width) ? (height - (height - width)) : height;
        int cropW = (width - height) / 2;
        cropW = (cropW < 0) ? 0 : cropW;
        int cropH = (height - width) / 2;
        cropH = (cropH < 0) ? 0 : cropH;

        return Bitmap.createBitmap(bitmap, cropW, cropH, newWidth, newHeight);
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
                                       DialogInterface.OnClickListener yesListener) {
        confirmarAccion(context, title, message, yesListener, null, android.R.string.no);
    }

    public static void confirmarAccion(Context context, String title, String message,
                                       DialogInterface.OnClickListener yesListener,
                                       DialogInterface.OnClickListener noListener,
                                       int negativeButtonStringId) {
        new AlertDialog.Builder(context)
                .setTitle(title)
                .setMessage(message)
                .setIcon(android.R.drawable.ic_dialog_alert)
                .setPositiveButton(android.R.string.yes, yesListener)
                .setNegativeButton(negativeButtonStringId, noListener)
                .show();
    }


    public static boolean validarFecha(int dia, int mes, int anio) {
        int diaLimite = 31;
        switch (mes) {
            case 2:
                if (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0))
                    diaLimite = 29;
                else diaLimite = 28;
                break;
            case 4:case 6:case 9:case 11:
                diaLimite = 30;
        }
        return !(dia <= 0 || dia > diaLimite || mes <= 0);
    }
}
