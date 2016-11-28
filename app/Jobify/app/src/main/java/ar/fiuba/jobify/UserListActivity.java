package ar.fiuba.jobify;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.LayoutRes;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.ActionBar;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;

import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;

import ar.fiuba.jobify.app_server_api.AllUsersResponse;
import ar.fiuba.jobify.app_server_api.BusquedaRequest;
import ar.fiuba.jobify.app_server_api.BusquedaResponse;
import ar.fiuba.jobify.app_server_api.Contact;
import ar.fiuba.jobify.app_server_api.ContactsResponse;
import ar.fiuba.jobify.app_server_api.ConversationsResponse;
import ar.fiuba.jobify.app_server_api.User;
import ar.fiuba.jobify.shared_server_api.ResponseMetadata;
import ar.fiuba.jobify.utils.RequestQueueSingleton;
import ar.fiuba.jobify.utils.Utils;

public class UserListActivity extends NavDrawerActivity {

    private final String LOG_TAG = UserListActivity.class.getSimpleName();

    private final static String package_name = "ar.fiuba.jobify.USER_LIST";
    public final static String LIST_MODE_MESSAGE = package_name+"_MODE_MESSAGE";
//    public final static String _PARAMETER = package_name+_X;

    public final static int MODE_NONE = 0;
    public final static int MODE_SOLICITUDES = 1;
    public final static int MODE_MOST_POPULAR = 2;
    public final static int MODE_BUSQUEDA = 3;
    public final static int MODE_CONVERSACIONES = 4;
//    public final static int[] ModeOptions = { MODE_NONE, MODE_SOLICITUDES, MODE_MOST_POPULAR,
//                                              MODE_BUSQUEDA, MODE_CONVERSACIONES };

    public final static String BUSQUEDA_REQUEST_MESSAGE = package_name+"_BUSQUEDA_REQUEST_MESSAGE";

    private final static int DEFAULT_LIST_SIZE = 20;
    private final static int PAGE_SIZE = 10;
    // Si busco más, crecer esta variable.
    private int mExpectedListSize = DEFAULT_LIST_SIZE;

    // Variable setteada con el totalCount del Metadata para saber cuántos resultados hay.
    public static int MAX_RESULTADOS = 100;
    private int cantResultados = MAX_RESULTADOS;

    private BusquedaRequest mBusquedaReq = null;

    private ArrayList<User> mUserArray = new ArrayList<>();
    private UserArrayAdapter mUserArrayAdapter;
    private EndlessScrollListener mEndlessScrollListener;
    int mode = MODE_NONE;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_list_drawer);

        ActionBar sab = getSupportActionBar();
        if (sab != null) sab.setDisplayHomeAsUpEnabled(true);

        ListView listView = (ListView) findViewById(R.id.user_list);
        if (listView == null) {
            Log.e(LOG_TAG, "No se encontró la listview de userlist!!!!!");
            return;
        }
        mUserArrayAdapter = new UserArrayAdapter(new ArrayList<User>());
        listView.setAdapter(mUserArrayAdapter);

        // Obtengo el modo
        Intent intent = getIntent();
        if (intent != null && intent.hasExtra(LIST_MODE_MESSAGE)) {
            mode = intent.getIntExtra(LIST_MODE_MESSAGE, mode);
        }

        if (mode == MODE_CONVERSACIONES) {
            final Activity act = this;
            listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    // Al cliquear una conversación con alguien, ir al chat con esa persona
                    User clickedUser = mUserArrayAdapter.getItem(position);
                    Utils.iniciarConversacionActivity(act, clickedUser.getId());
                }
            });
        } else {
            listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    // Al cliquear un usuario, abrir su perfil
                    User clickedUser = mUserArrayAdapter.getItem(position);
                    startActivity(
                            new Intent(UserListActivity.this, PerfilActivity.class)
                                    .putExtra(PerfilActivity.FETCHED_USER_ID_MESSAGE, clickedUser.getId())
                    );
                }
            });
        }

        switch (mode) {
            case MODE_SOLICITUDES:
                if (sab != null)
                    sab.setTitle("Solicitudes pendientes");
                esconderBotonDeBusqueda();
                showProgress(true);
                listarSolicitudesReceived();
                break;
            case MODE_MOST_POPULAR:
                if (sab != null)
                    sab.setTitle("Profesionales más populares");
                showProgress(true);
                listView.setOnScrollListener(mEndlessScrollListener = new EndlessScrollListener());
//                listarMasPopulares();
                listarTodosLosUsuarios();// TODO: Cambiar por el de arriba una vez que funcione ese
                break;
            case MODE_BUSQUEDA:
                if (sab != null)
                    sab.setTitle("Resultados");
                showProgress(true);
                listView.setOnScrollListener(mEndlessScrollListener = new EndlessScrollListener());
                generarBusqueda();
                break;
            case MODE_CONVERSACIONES:
                if (sab != null)
                    sab.setTitle("Conversaciones");
                showProgress(true);
                esconderBotonDeBusqueda();
                listarConversaciones();
                break;
            case MODE_NONE:
            default:
        }
    }

    @Override
    public void setContentView(@LayoutRes int layoutResID) {
        super.setContentView(layoutResID);
        onCreateDrawer(R.id.user_list_toolbar, R.id.user_list_drawer_layout, R.id.user_list_nav_view);
    }

    public void onStop() {
        super.onStop();
        if (RequestQueueSingleton.hasRequestQueue()) {
            RequestQueue mRequestQueue = RequestQueueSingleton
                    .getInstance(this.getApplicationContext())
                    .getRequestQueue();
            mRequestQueue.cancelAll(LOG_TAG);
        }
    }

    private void esconderBotonDeBusqueda() {
        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.user_list_fab);
        if (fab != null) {
            fab.setVisibility(View.GONE);
        }
    }


    private void listarSolicitudesReceived() {
//        Toast.makeText(this, "Se listan las solicitudes pendientes", Toast.LENGTH_LONG)
//                .show();

        final Context ctx = this;
        String urlContactos = Utils.getAppServerUrl(this, connectedUserID, getString(R.string.get_contacts_path));

        Utils.fetchJsonFromUrl(this, Request.Method.GET, urlContactos, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        ContactsResponse contactsResponse =
                                ContactsResponse.parseJSON(response.toString());

                        if (contactsResponse == null) {
                            Toast.makeText(ctx, "Ha ocurrido un error", Toast.LENGTH_LONG)
                                    .show();
                            Log.e(LOG_TAG, "ContactsResponse null");
                            mostrarNoHayResultados();
                            return;
                        }

                        ArrayList<Contact> contactsReceived =
                                contactsResponse.getContactsWithStatus(Contact.Status.RECEIVED);

                        int cant = contactsReceived.size();
                        cantResultados = cant < MAX_RESULTADOS ? cant : MAX_RESULTADOS;
                        mExpectedListSize = cant < mExpectedListSize ? cant : mExpectedListSize;

                        if (mExpectedListSize == 0) {
                            mostrarNoHayResultados();

                        } else {
                            for (Contact c : contactsReceived) {
                                if (c != null)
                                    agregarResultado(new User(c));
                            }
                        }
                    }
                }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        if (error.networkResponse != null) {
                            String sc = Utils.statusCodeString(error.networkResponse.statusCode);
                            Log.e(LOG_TAG, "Contactos: " + sc);
                            Toast.makeText(ctx, sc, Toast.LENGTH_LONG)
                                    .show();
                        }
                        error.printStackTrace();

                        showProgress(false);
                    }
                }, LOG_TAG);
    }

    /// de prueba //;//
    private void listarTodosLosUsuarios() {
        Toolbar toolbar = (Toolbar) findViewById(R.id.user_list_toolbar);
        if (toolbar != null)
            toolbar.setTitle("Todos los usuarios");
        else
            Log.w(LOG_TAG, "No pude encontrar toolbar para settear título");

        Utils.getJsonFromAppServer(this, getString(R.string.get_all_users_path),
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        AllUsersResponse allUsersResponse =
                                AllUsersResponse.parseJSON(response.toString());

                        if (allUsersResponse == null) {
                            Log.e(LOG_TAG, "Null parsed JSON from all_users");
                            return;
                        }

                        int cant = 0;
                        ResponseMetadata meta = allUsersResponse.getMetadata();
                        if (meta != null) {
                            cant = Long.valueOf(meta.getCount()).intValue();
                        } else
                            Log.e(LOG_TAG, "AllUsersResponse Metadata null!");
                        cantResultados = cant < MAX_RESULTADOS ? cant : MAX_RESULTADOS;
                        mExpectedListSize = cant < mExpectedListSize ? cant : mExpectedListSize;

                        if (mExpectedListSize == 0) {
                            mostrarNoHayResultados();
                        } else {
                            for (long id : allUsersResponse.getAllUsers()) {
                                fetchAndAddUser(id);
                            }
                        }
                    }
        }, LOG_TAG);
    }
    ///

    private void listarMasPopulares() {
        mBusquedaReq = new BusquedaRequest();
        cargarPageDeUsuarios(0, false);
    }

    public void generarBusqueda() {
        String busquedaMensaje;

        Intent intent = getIntent();
        if (intent != null && intent.hasExtra(BUSQUEDA_REQUEST_MESSAGE)) {
            busquedaMensaje = intent.getStringExtra(BUSQUEDA_REQUEST_MESSAGE);
            if (busquedaMensaje == null) busquedaMensaje = "";
        } else {
            Log.e(LOG_TAG, "No encontré intent o mensaje para hacer búsqueda");
            return;
        }

        mBusquedaReq = BusquedaRequest.parseJSON(busquedaMensaje);
        if (mBusquedaReq == null) {
            Log.e(LOG_TAG, "Error de Json BusquedaRequest");
            return;
        }

        cargarPageDeUsuarios(0, false);
    }

    public void listarConversaciones() {
        final Context ctx = this;
        String urlConversaciones = Utils.getAppServerUrl(this, connectedUserID, getString(R.string.get_conversations_path));
        Utils.fetchJsonFromUrl(this, Request.Method.GET, urlConversaciones, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        ConversationsResponse convResponse =
                                ConversationsResponse.parseJSON(response.toString());

                        if (convResponse == null) {
                            Toast.makeText(ctx, "Ha ocurrido un error", Toast.LENGTH_LONG)
                                    .show();
                            Log.e(LOG_TAG, "ConversationsResponse null");
                            mostrarNoHayResultados();
                            return;
                        }

                        ResponseMetadata meta = convResponse.getMetadata();
                        if (meta != null) {
                            mExpectedListSize = Long.valueOf(meta.getTotalCount()).intValue();
                        } else
                            Log.e(LOG_TAG, "ConversationsResponse Metadata null!");
                        if (mExpectedListSize == 0) {
                            mostrarNoHayResultados();

                        } else {
                            for (User user : convResponse.getConversations()) {
                                if (user != null)
                                    agregarResultado(user);
                            }
                        }
                    }
                }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        if (error.networkResponse != null) {
                            String sc = Utils.statusCodeString(error.networkResponse.statusCode);
                            Log.e(LOG_TAG, "Listar conversaciones: " + sc);
                            Toast.makeText(ctx, sc, Toast.LENGTH_LONG)
                                    .show();
                        }
                        error.printStackTrace();
                        showProgress(false);
                    }
                }, LOG_TAG);
    }



    private List<User> ordenarPorPopularidad(List<User> lista) {
        Collections.sort(lista, new Comparator<User>() {
            @Override
            public int compare(User u1, User u2) {
                return -(Long.valueOf(u1.getCantRecomendaciones())
                        .compareTo(u2.getCantRecomendaciones()));
            }
        });
        return lista;
    }


    /**
     * Shows the progress UI and hides the login form.
     */
    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private void showProgress(final boolean show) {
        final ListView listView = (ListView) findViewById(R.id.user_list);
        final View progressView = findViewById(R.id.busqueda_progress);
        if (listView == null || progressView == null) {
            Log.e(LOG_TAG, "No pude encontrar la lista de usuarios o el progress loader.");
            return;
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
            int shortAnimTime = getResources().getInteger(android.R.integer.config_shortAnimTime);

            listView.setVisibility(show ? View.GONE : View.VISIBLE);
            listView.animate().setDuration(shortAnimTime).alpha(
                    show ? 0 : 1).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    listView.setVisibility(show ? View.GONE : View.VISIBLE);
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
            listView.setVisibility(show ? View.GONE : View.VISIBLE);
        }
    }

    @Deprecated
    private void fetchAndAddUser(long id) {
        Utils.getJsonFromAppServer(this, getString(R.string.get_user_path), id,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        agregarResultado(User.parseJSON(response.toString()));
                    }
                }, LOG_TAG);
    }

    private void mostrarNoHayResultados() {
        Utils.showView(this, R.id.userlist_sin_resultados_layout);
        showProgress(false);
    }

    /**
     * Carga los siguientes PAGE_SIZE resultados de una búsqueda con mBusquedaReq.
     */
    private boolean cargarPageDeUsuarios(int pageNumber, boolean forzarCarga) {
        if (mBusquedaReq == null) {
            Log.e(LOG_TAG, "BusquedaRequest null, abort");
            return false;
        }
        if (!forzarCarga && (pageNumber * PAGE_SIZE >= cantResultados)) {
            if (mEndlessScrollListener != null)
                mEndlessScrollListener.desactivar();
            return false;
        }
        //showProgress(true);//TODO?

        final Context ctx = this;

        int numFirst = (pageNumber * PAGE_SIZE + 1);
        int numLast  = (pageNumber + 1) * PAGE_SIZE;
        HashMap<String, String> map = new HashMap<>();
        map.put(getString(R.string.get_busqueda_users_first_query), Long.toString(numFirst));
        map.put(getString(R.string.get_busqueda_users_last_query),  Long.toString(numLast));
        String urlBusqueda = Utils.getAppServerUrl(this, getString(R.string.get_search_path), map);

        // TODO: Revisar
        Utils.fetchJsonFromUrl(this, Request.Method.GET, urlBusqueda, mBusquedaReq.toJsonObject(),
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        BusquedaResponse busqResponse = BusquedaResponse.parseJSON(response.toString());

                        if (busqResponse == null) {
                            Toast.makeText(ctx, "Ha ocurrido un error", Toast.LENGTH_LONG)
                                    .show();
                            Log.e(LOG_TAG, "BusquedaResponse null");
                            mostrarNoHayResultados();
                            return;
                        }

                        int cant = 0;
                        ResponseMetadata meta = busqResponse.getMetadata();
                        if (meta != null)
                            cant = Long.valueOf(meta.getCount()).intValue();
                        else
                            Log.e(LOG_TAG, "BusquedaResponse Metadata null!");
                        cantResultados = cant < MAX_RESULTADOS ? cant : MAX_RESULTADOS;
                        mExpectedListSize = cant < mExpectedListSize ? cant : mExpectedListSize;

                        if (mExpectedListSize == 0) {
                            mostrarNoHayResultados();

                        } else {
                            for (User user : busqResponse.getUsers()) {
                                if (user != null)
                                    agregarResultado(user);
                            }
                            mEndlessScrollListener.activar();
                        }
                    }
                }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Toast.makeText(ctx, "Todavía no implementado / error", Toast.LENGTH_LONG)
                                .show();//
                        if (error.networkResponse != null) {
                            String sc = Utils.statusCodeString(error.networkResponse.statusCode);
                            Log.e(LOG_TAG, "Búsqueda: " + sc);
                            Toast.makeText(ctx, sc, Toast.LENGTH_LONG)
                                    .show();
                        }
                        error.printStackTrace();

                        showProgress(false);
                    }
                }, LOG_TAG);

        return true;
    }

    // Agrega un user fetcheado a los resultados
    // Si llega a la cantidad esperada, los muestra.
    private void agregarResultado(User user) {
        if (user == null)
            mExpectedListSize--;
        else
            mUserArray.add(user);

        if (mUserArray.size() >= mExpectedListSize) {
            mUserArrayAdapter.addAll(ordenarPorPopularidad(mUserArray));
            showProgress(false); // Escondo progress loader
            mUserArray.clear();
        }
    }

    public void irABusquedaActivity(View v) {
        // Volver a la búsqueda si vengo de una? Ocultar botón si se trata de los contactos propios.
        startActivity(new Intent(this, BusquedaActivity.class));
    }


    private class UserArrayAdapter extends ArrayAdapter<User> {

        public UserArrayAdapter(List<User> userList) {
            super(UserListActivity.this, R.layout.user_list_item, userList);
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            View itemView = convertView;
            if (itemView == null) {
                itemView = LayoutInflater.from(getContext())
                        .inflate(R.layout.user_list_item, parent, false);
            }

            // El modo conversaciones muestra unreadCount
            // en vez de trabajo actual y cantidad de recomendaciones
            boolean usuarioReducido = true;
            if (mode != MODE_CONVERSACIONES)
                usuarioReducido = false;

            User user = getItem(position);
            if (user != null) {
                ImageView iv_thumbnail = (ImageView) itemView.findViewById(R.id.list_item_thumbnail);
                TextView tv_nombre  = (TextView) itemView.findViewById(R.id.list_item_nombre);
                TextView tv_trabajo = (TextView) itemView.findViewById(R.id.list_item_trabajo);
                TextView tv_recom   = (TextView) itemView.findViewById(R.id.list_item_recomendaciones);
                TextView tv_unread  = (TextView) itemView.findViewById(R.id.list_item_unread_messages);

                if (iv_thumbnail != null) {
                    Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(getContext())).buildUpon()
                            .appendPath(getString(R.string.get_thumbnail_path))
                            .appendPath(Long.toString(user.getId()))
                            .build();
                    final String url = builtUri.toString();

                    Utils.cargarImagenDeURLenImageView(getContext(),
                            iv_thumbnail, url, LOG_TAG, true);
                }

                if (tv_nombre != null)
                    tv_nombre.setText(user.getFullName());
                // Para listar conversaciones
                if (usuarioReducido) {
                    if (tv_trabajo != null)
                        tv_trabajo.setVisibility(View.GONE);
                    if (tv_recom != null)
                        tv_recom.setVisibility(View.GONE);
                    if (tv_unread != null) {
                        long cantUnread = user.getUnreadCount();
                        if (cantUnread > 0) {
                            tv_unread.setText(String.format(Locale.US, "%d", cantUnread));
                            tv_unread.setVisibility(View.VISIBLE);
                        }
                    }
                // Los demás casos
                } else {
                    if (tv_trabajo != null)
                        tv_trabajo.setText(user.getUltimoTrabajoActual());
                    if (tv_recom != null) {
                        long cantRecom = user.getCantRecomendaciones();
                        if (cantRecom == 0)
                            tv_recom.setVisibility(View.GONE);
                        else
                            tv_recom.setText(String.format(Locale.US, "%d", cantRecom));
                    }
                }
            }
            return itemView;
        }
    }

    public class EndlessScrollListener implements AbsListView.OnScrollListener {

        private int visibleThreshold = 3;
        private int currentPage = 0;
        private int previousTotal = 0;
        private boolean loading = true;
        private boolean activado = true;

        public EndlessScrollListener() {}

//        public EndlessScrollListener(int visibleThreshold) {
//            this.visibleThreshold = visibleThreshold;
//        }

        public void activar() {
            this.activado = true;
            this.loading = true;
        }
        public void desactivar() {
            this.activado = false;
            this.loading = false;
        }

        @Override
        public void onScroll(AbsListView view, int firstVisibleItem,
                             int visibleItemCount, int totalItemCount) {
            if (!activado) return;

            if (loading) {
                if (totalItemCount > previousTotal) {
                    loading = false;
                    previousTotal = totalItemCount;
                    currentPage++;
                }
            }
            if (!loading && firstVisibleItem != 0
                    && (totalItemCount - visibleItemCount) <= (firstVisibleItem + visibleThreshold)) {
                if (cargarPageDeUsuarios(currentPage, false))
                    loading = true;
            }
        }

        @Override
        public void onScrollStateChanged(AbsListView view, int scrollState) {
        }
    }
}
