package ar.fiuba.jobify.utils;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.NotificationManager;
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
import android.support.annotation.DrawableRes;
import android.support.annotation.IdRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
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

import ar.fiuba.jobify.ConversacionActivity;
import ar.fiuba.jobify.PerfilActivity;
import ar.fiuba.jobify.R;
import ar.fiuba.jobify.UserListActivity;

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

    public static void limpiarNotificaciones(Context ctx) {
        NotificationManager mNotificationManager =
                (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
        mNotificationManager.cancelAll();
    }

    /////////////////////////////////////////// FETCHER ////////////////////////////////////////////

    public interface ServerStatusCode {
        int OK = 200,
            OK_CREATED = 201,
            OK_ACCEPTED = 202,
            OK_NOCONTENT = 204,

            BADREQUEST = 400,
            UNAUTHORIZED = 401,
            FORBIDDEN = 403,
            NOTFOUND = 404,
            CONFLICT = 409,
            GONE = 410,

            INTERNALERROR = 500,
            NOTIMPLEMENTED = 501,
            BADGATEWAY = 502;
    }

    @NonNull
    public static String statusCodeString(int statusCode) {
        switch (statusCode) {
            case ServerStatusCode.OK:             return "OK";
            case ServerStatusCode.OK_CREATED:     return "Creado";
            case ServerStatusCode.OK_ACCEPTED:    return "Aceptado";
            case ServerStatusCode.OK_NOCONTENT:   return "No content";
            case ServerStatusCode.BADREQUEST:     return "Bad request";
            case ServerStatusCode.UNAUTHORIZED:   return "No autorizado";
            case ServerStatusCode.FORBIDDEN:      return "Prohibido";
            case ServerStatusCode.NOTFOUND:       return "No encontrado";
            case ServerStatusCode.CONFLICT:       return "Conflicto";
            case ServerStatusCode.GONE:           return "Gone";
            case ServerStatusCode.INTERNALERROR:  return "Error interno del server";
            case ServerStatusCode.NOTIMPLEMENTED: return "No implementado";
            case ServerStatusCode.BADGATEWAY:     return "Bad gateway";
            default:
                return "Error inesperado";
        }
    }


    public static String getAppServerBaseURL(Context ctx) {
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(ctx);
        String ip = sharedPref.getString(ctx.getString(R.string.pref_app_server_ip), ctx.getString(R.string.pref_default_appServer_ip));
        String puerto = sharedPref.getString(ctx.getString(R.string.pref_app_server_port), ctx.getString(R.string.pref_default_appServer_puerto));

        try {
            if (Integer.parseInt(puerto) <= 0)
                return "http://" + ip + "/";
        } catch (NumberFormatException ex) {
            Log.e(LOG_TAG, "Puerto no es un número");
        }

        return "http://" + ip + ":" + puerto + "/";
    }

    @NonNull
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
        for (String path : paths) {
            if (path != null)
                builder = builder.appendPath(path);
        }
        Uri builtUri = builder.appendPath(Long.toString(idFetched)).build();
        return builtUri.toString();
    }

    /**
     * Devuelve un String con la URL para utilizar en un fetch, en la forma:
     * {@code http://<AppServer>/<paths/separados/por/barras>}
     */
    public static String getAppServerUrl(Context ctx, String ... paths) {
        Uri.Builder builder = Uri.parse(Utils.getAppServerBaseURL(ctx)).buildUpon();
        for (String path : paths) {
            if (path != null)
                builder = builder.appendPath(path);
        }
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

//    public static void getJsonFromAppServer(Context context, String getPathSegment,
//                                            Response.Listener<JSONObject> responseListener,
//                                            final String logTag) {
//
//        getJsonFromUrl(context, getAppServerUrl(context, getPathSegment), null, responseListener,
//                logTag);
//    }

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

    public static void fetchJsonFromUrl(final Context context, int method, final String url,
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
                        if (error.networkResponse != null
                                && error.networkResponse.statusCode == ServerStatusCode.OK) {
                            try {
                                responseListener.onResponse(new JSONObject("{}"));
                            } catch (JSONException ex) {
                                Log.e(LOG_TAG, "JSON vacío no aceptado");
                            }
                        } else {
                            Log.e(logTag, "Error Listener. URL: " + url);
                            if (error.networkResponse != null) {
                                String sc = statusCodeString(error.networkResponse.statusCode);
                                Log.e(LOG_TAG, "Fetch error: " + sc);
                            }
//                            error.printStackTrace();
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
                        headers.put("Connection", "close"); //Te amo, header que soluciona cosas ~ mc
                        String token = getToken(context);
                        if (token != null)
                            headers.put("Authorization", token);
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


    public static boolean cargarImagenDeURLenImageView(final Context ctx, final ImageView imageView,
                                                       final String  url, final String logTag) {
        return cargarImagenDeURLenImageView(ctx, imageView, url, logTag, false);
    }

    // Devuelve false si no se encontró el ImageView
    // Asumo que si se le carga una imagen se la quiere ver, por lo que cambia visibilidad!
    public static boolean cargarImagenDeURLenImageView(final Context ctx, final ImageView imageView,
                                  final String url, final String logTag, final boolean squareCrop) {
        if (imageView == null) {
            Log.e(logTag, "No pude encontrar el ImageView, no cargo imagen. ("+url+")");
            return false;
        }

        ImageRequest request = new ImageRequest(url,
                new Response.Listener<Bitmap>() {
                    @Override
                    public void onResponse(Bitmap bitmap) {
                        imageView.setVisibility(View.INVISIBLE);
                        if (squareCrop)
                            imageView.setImageBitmap(cropToSquare(bitmap));
                        else
                            imageView.setImageBitmap(bitmap);
                        animateViewVisibility(imageView, true);
                    }
                }, imageView.getWidth(), imageView.getHeight(),
                ImageView.ScaleType.CENTER_INSIDE, null,

                new Response.ErrorListener() {
                    @SuppressWarnings("deprecation")
                    public void onErrorResponse(VolleyError error) {
                        if (error.networkResponse == null) {
                            Log.e(logTag, "Error de response, no pude cargar la imagen." +
                                    " (url: " + url + ")");
                            return;
                        }
                        if (error.networkResponse.statusCode == ServerStatusCode.OK) {
//                            Log.e(logTag, "Problema con la imagen. Re-request");//
                            cargarImagenDeURLenImageView(ctx, imageView, url, logTag);
                            return;
                        }
                        switch (error.networkResponse.statusCode) {
                            case ServerStatusCode.NOTFOUND:
                            case ServerStatusCode.OK_NOCONTENT:
                                Log.d(logTag, "Imagen no existe");
                                break;
                            default:
                                Log.d(logTag, "Error cargando imagen: " +
                                        statusCodeString(error.networkResponse.statusCode));
                        }

                        @DrawableRes int drawableId = R.drawable.ic_person;
                        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
                            imageView.setImageDrawable(ctx.getDrawable(drawableId));
                        } else {
                            imageView.setImageDrawable(ctx.getResources().getDrawable(drawableId));
                        }
                    }
                }) {
                    @Override
                    public Map<String, String> getHeaders() throws AuthFailureError {
                        Map<String, String> headers = new HashMap<>();
                        String token = getToken(ctx);
                        if (token != null)
                            headers.put("Authorization", token);
                        return headers;
                    }
        };
        RequestQueueSingleton.getInstance(ctx)
                .addToRequestQueue(request);
        return true;
    }

    // TODO: esto es un malformado semiclon de la función de arriba;
    // TODO: semi-fea reutilización de código, pero hay que cambiar varias cosas para refactorizar.
    // Asumo que si se le carga una imagen se la quiere ver, por lo que cambia visibilidad!
    public static void cargarImagenDeURLenImageView(final Activity act, final @IdRes int iv_id,
                                  final String url, final String logTag, final boolean squareCrop) {

        ImageRequest request = new ImageRequest(url,
                new Response.Listener<Bitmap>() {
                    @Override
                    public void onResponse(Bitmap bitmap) {
                        ImageView imageView = (ImageView) act.findViewById(iv_id);
                        if (imageView == null) {
                            Log.e(logTag, "No pude post-encontrar ImageView, no cargo imagen");
                            return;
                        }
                        if (squareCrop)
                            imageView.setImageBitmap(cropToSquare(bitmap));
                        else
                            imageView.setImageBitmap(bitmap);
                        imageView.setVisibility(View.VISIBLE);
                    }
                }, 0, 0,
                ImageView.ScaleType.CENTER_INSIDE, null,

                new Response.ErrorListener() {
                    @SuppressWarnings("deprecation")
                    public void onErrorResponse(VolleyError error) {
                        if (error.networkResponse == null) {
                            Log.e(logTag, "Error de response, no pude cargar la imagen." +
                                    " (url: " + url + ")");
                            return;
                        }
                        if (error.networkResponse.statusCode == ServerStatusCode.OK) {
//                            Log.e(logTag, "Problema con la imagen. Re-request");//
                            cargarImagenDeURLenImageView(act, iv_id, url, logTag, squareCrop);
                            return;
                        }
                        switch (error.networkResponse.statusCode) {
                            case ServerStatusCode.NOTFOUND:
                            case ServerStatusCode.OK_NOCONTENT:
                                Log.d(logTag, "Imagen no existe");
                                break;
                            default:
                                Log.d(logTag, "Error cargando imagen: " +
                                        statusCodeString(error.networkResponse.statusCode));
                        }

                        ImageView imageView = (ImageView) act.findViewById(iv_id);
                        if (imageView == null) {
                            Log.e(logTag, "No pude post-encontrar ImageView, ni en el error!");
                            return;
                        }
                        @DrawableRes int drawableId = R.drawable.ic_person;
                        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
                            imageView.setImageDrawable(act.getDrawable(drawableId));
                        } else {
                            imageView.setImageDrawable(act.getResources().getDrawable(drawableId));
                        }
                    }
                }) {
            @Override
            public Map<String, String> getHeaders() throws AuthFailureError {
                Map<String, String> headers = new HashMap<>();
                String token = getToken(act);
                if (token != null)
                    headers.put("Authorization", token);
                return headers;
            }
        };
        RequestQueueSingleton.getInstance(act)
                .addToRequestQueue(request);
    }

    // De nuevo, código repetido de fin de curso, pero es bastante difícil no repetirlo.
    // Usada específicamente para cargar imágenes en resultados de UserArrayAdapter
    public static boolean cargarImagenDeURL(final Context ctx, final long id, final ImageView imageView,
                                            final UserListActivity.UserArrayAdapter adapter) {
        final String url = getAppServerUrl(ctx, id, ctx.getString(R.string.get_thumbnail_path));

        ImageRequest request = new ImageRequest(url,
                new Response.Listener<Bitmap>() {
                    @Override
                    public void onResponse(Bitmap bitmap) {
                        bitmap = cropToSquare(bitmap);
                        imageView.setImageBitmap(bitmap);
                        imageView.setVisibility(View.VISIBLE);
                        adapter.guardarImagen(id, bitmap);
                    }
                }, imageView.getWidth(), imageView.getHeight(),
                ImageView.ScaleType.CENTER_INSIDE, null,

                new Response.ErrorListener() {
                    @SuppressWarnings("deprecation")
                    public void onErrorResponse(VolleyError error) {
                        if (error.networkResponse == null) {
                            Log.w(LOG_TAG, "Error de response, no pude cargar la imagen." +
                                    " (url: " + url + ")");
                            return;
                        }
                        if (error.networkResponse.statusCode == ServerStatusCode.OK) {
//                            Log.e(logTag, "Problema con la imagen. Re-request");//
                            cargarImagenDeURL(ctx, id, imageView, adapter);
                            return;
                        }
                        switch (error.networkResponse.statusCode) {
                            case ServerStatusCode.NOTFOUND:
                            case ServerStatusCode.OK_NOCONTENT:
                                Log.w(LOG_TAG, "Imagen no existe");
                                break;
                            default:
                                Log.w(LOG_TAG, "Error cargando imagen: " +
                                        statusCodeString(error.networkResponse.statusCode));
                        }

                        @DrawableRes int drawableId = R.drawable.ic_person;
                        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
                            imageView.setImageDrawable(ctx.getDrawable(drawableId));
                        } else {
                            imageView.setImageDrawable(ctx.getResources().getDrawable(drawableId));
                        }
                    }
                }) {
            @Override
            public Map<String, String> getHeaders() throws AuthFailureError {
                Map<String, String> headers = new HashMap<>();
                String token = getToken(ctx);
                if (token != null)
                    headers.put("Authorization", token);
                return headers;
            }
        };
        RequestQueueSingleton.getInstance(ctx)
                .addToRequestQueue(request);
        return true;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////

    public static @IdRes int[] perfilVisibilityViews = {
            R.id.perfil_nombre_editable_frame, R.id.text_perfil_trabajo_actual,
            R.id.text_perfil_ciudad, R.id.text_perfil_ciudad_editable_wrapper,
            R.id.text_perfil_ciudad_editable, R.id.boton_perfil_location,
            R.id.text_perfil_resumen,
            R.id.text_perfil_nacimiento, R.id.perfil_nacimiento_editable,
            R.id.text_perfil_resumen_editable_wrapper, R.id.perfil_experiencia_laboral_list,
            R.id.perfil_experiencia_laboral_list_editable, R.id.perfil_experiencia_laboral_list_new,
            R.id.perfil_skills_list, R.id.perfil_skills_list_editable, R.id.perfil_skills_list_new
    };
//    R.id.text_perfil_cant_recomendaciones se maneja por separado

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
//        EditText et = (EditText) activity.findViewById(idRes);
        TextView et = (TextView) activity.findViewById(idRes);
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

    public static void showView(AppCompatActivity activity, @IdRes int idRes) {
        View v = activity.findViewById(idRes);
        if (v != null)
            v.setVisibility(View.VISIBLE);
    }

    public static void hideView(AppCompatActivity activity, @IdRes int idRes) {
        View v = activity.findViewById(idRes);
        if (v != null)
            v.setVisibility(View.GONE);
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    public static void animateViewVisibility(final View v, final boolean show) {
        if (v == null)
            return;
        // On Honeycomb MR2 we have the ViewPropertyAnimator APIs, which allow
        // for very easy animations. If available, use these APIs to fade-in
        // the progress spinner.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
            int shortAnimTime = 200;
            v.setVisibility(show ? View.VISIBLE : View.GONE);
            v.animate().setDuration(shortAnimTime).alpha(
                    show ? 1 : 0).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    v.setVisibility(show ? View.VISIBLE : View.GONE);
                }
            });

        } else {
            // The ViewPropertyAnimator APIs are not available,
            // so simply show and hide the relevant UI components.
            v.setVisibility(show ? View.VISIBLE : View.GONE);
        }
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


    public static Bitmap normalizarBitmap(Bitmap bitmap) {
        int MAX = 1000; // Máximo en cualquier dimensión
        int newWidth = bitmap.getWidth() > MAX ? MAX : bitmap.getWidth();
        int newHeight = (int) Math.round((1.0 * bitmap.getHeight() / bitmap.getWidth()) * newWidth);
        return Bitmap.createScaledBitmap(bitmap, newWidth, newHeight, true);
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
            String token = getToken(ctx);
            if (token != null)
                headers.put("Authorization", token);
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
    @Nullable
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

    public static String getToken(Context ctx) {
        SharedPreferences sharedPref = ctx.getSharedPreferences(ctx.getString(R.string.shared_pref_connected_user), 0);
        return sharedPref.getString(ctx.getString(R.string.stored_connected_user_token), null);
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
        confirmarAccion(context, title, message, yesListener, null,
                        android.R.string.yes, android.R.string.no);
    }

    public static void confirmarAccion(Context context, String title, String message,
                                       DialogInterface.OnClickListener yesListener,
                                       DialogInterface.OnClickListener noListener,
                                       int positiveButtonStringId,
                                       int negativeButtonStringId) {
        new AlertDialog.Builder(context)
                .setTitle(title)
                .setMessage(message)
                .setIcon(android.R.drawable.ic_dialog_alert)
                .setPositiveButton(positiveButtonStringId, yesListener)
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
