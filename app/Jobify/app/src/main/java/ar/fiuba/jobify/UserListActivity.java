package ar.fiuba.jobify;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.LayoutRes;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.ActionBar;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.RequestQueue;
import com.android.volley.Response;

import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.app_server_api.AllUsersResponse;
import ar.fiuba.jobify.app_server_api.Contact;
import ar.fiuba.jobify.app_server_api.ContactsResponse;
import ar.fiuba.jobify.app_server_api.User;

public class UserListActivity extends NavDrawerActivity {

    private final String LOG_TAG = UserListActivity.class.getSimpleName();

    private final static String package_name = "ar.fiuba.jobify.USER_LIST";
    public final static String LIST_MODE = package_name+"_MODE";
//    public final static String _PARAMETER = package_name+_TODO;

    public final static int MODE_NONE = 0;
    public final static int MODE_SOLICITUDES = 1;
    public final static int MODE_ALL_USERS = 2;
    public final static int MODE_BUSQUEDA = 3;
//    public final static int[] ModeOptions = { MODE_NONE, MODE_SOLICITUDES, MODE_ALL_USERS, MODE_BUSQUEDA };

    private UserArrayAdapter mUserArrayAdapter;
    int mode = MODE_NONE;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_list_drawer);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        if (fab != null)
            fab.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    // Volver a la búsqueda? Ocultar si se trata de los contactos propios.
                    Snackbar.make(view, "Empezar nueva búsqueda", Snackbar.LENGTH_LONG)
                            .setAction("Action", null).show(); // TODO
                }
            });
        ActionBar sab = getSupportActionBar();
        if (sab != null) sab.setDisplayHomeAsUpEnabled(true);


        // Obtengo el modo
        Intent intent = getIntent();
        if (intent != null && intent.hasExtra(LIST_MODE)) {
            mode = intent.getIntExtra(LIST_MODE, mode);
        }

        ListView listView = (ListView) findViewById(R.id.user_list);
        if (listView == null) {
            Log.e(LOG_TAG, "No se encontró la listview de userlist!!!!!!!!!");
            return;
        }
        mUserArrayAdapter = new UserArrayAdapter(new ArrayList<User>());
        listView.setAdapter(mUserArrayAdapter);

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

        switch (mode) {
            case MODE_SOLICITUDES:
                listarSolicitudesReceived();
                break;
            case MODE_ALL_USERS:
                listarTodosLosUsuarios();
                break;
            case MODE_BUSQUEDA:
                // TODO
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
        if (RequestQueueSingleton.hasRequestQueue()) {  // TODO: Llamar a esto acá? Revisar.
            RequestQueue mRequestQueue = RequestQueueSingleton
                    .getInstance(this.getApplicationContext())
                    .getRequestQueue();
            mRequestQueue.cancelAll(LOG_TAG);
        }
    }


    private void listarSolicitudesReceived() {
        Toast.makeText(this, "Listo las solicitudes pendientes", Toast.LENGTH_LONG)
                .show();

        Utils.getJsonFromAppServer(this, getString(R.string.get_contacts_path), connectedUserID,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {

                        ContactsResponse contactsResponse = ContactsResponse.parseJSON(response.toString());
                        if (contactsResponse == null)
                            return;
                        ArrayList<Contact> contacts =
                                contactsResponse.getContactsWithStatus(Contact.Status.RECEIVED);

                        for ( Contact c : contacts ) {
                            fetchAndAddUser(c.getId());
                        }
                    }
        }, LOG_TAG);
    }

    // de prueba //;//
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
                            Log.d(LOG_TAG, "Null parsed JSON from all_users");
                            return;
                        }

                        for (long id : allUsersResponse.getAllUsers()) {
                            fetchAndAddUser(id);
                        }
                    }
        }, LOG_TAG);
    }

    private void fetchAndAddUser(long id) { // TODO: De prueba, CAMBIAR POR GET REDUCIDO

        Utils.getJsonFromAppServer(this, getString(R.string.get_user_path), id,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        User mUser = User.parseJSON(response.toString());
                        if (mUser != null) {
                            mUserArrayAdapter.add(mUser);
                        }
                    }
                }, LOG_TAG);
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

            User user = getItem(position);
            if (user != null) {
                ImageView iv_thumbnail = (ImageView) itemView.findViewById(R.id.list_item_thumbnail);
                TextView tv_nombre  = (TextView) itemView.findViewById(R.id.list_item_nombre);
                TextView tv_trabajo = (TextView) itemView.findViewById(R.id.list_item_trabajo);
                TextView tv_recom   = (TextView) itemView.findViewById(R.id.list_item_recomendaciones);

                if (iv_thumbnail != null) {
                    Uri builtUri = Uri.parse(Utils.getAppServerBaseURL(getContext())).buildUpon()
                            .appendPath(getString(R.string.get_thumbnail_path))
                            .appendPath(Long.toString(user.getId()))
                            .build();
                    final String url = builtUri.toString();

                    Utils.cargarImagenDeURLenImageView(getContext(),
                            iv_thumbnail, url, LOG_TAG);
                }

                if (tv_nombre != null)
                    tv_nombre.setText(user.getFullName());
                if (tv_trabajo != null)
                    tv_trabajo.setText(user.getTrabajosActuales());    // TODO: Revisar si cortar a una línea
                if (tv_recom != null)
                    tv_recom.setText(String.valueOf(user.getCantRecomendaciones()));
            }
            return itemView;
        }
    }
}
