package ar.fiuba.jobify;

import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.LayoutRes;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.ActionBar;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.android.volley.RequestQueue;

import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.app_server_api.User;

public class UserListActivity extends NavDrawerActivity {

    private final String LOG_TAG = UserListActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_list);
        Toolbar toolbar = (Toolbar) findViewById(R.id.perfil_toolbar);
        setSupportActionBar(toolbar);

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

        ListView listView = (ListView) findViewById(R.id.user_list);
        if (listView != null) {
            UserArrayAdapter userArrayAdapter =
                    new UserArrayAdapter(new ArrayList<User>());//TODO hardcodeo
            listView.setAdapter(userArrayAdapter);
        }
    }

    @Override
    public void setContentView(@LayoutRes int layoutResID) {
        super.setContentView(layoutResID);
        onCreateDrawer();
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
                    Uri builtUri = Uri.parse(PerfilActivity.getAppServerBaseURL()).buildUpon()
                            .appendPath(getString(R.string.perfil_get_thumbnail_path))
                            .appendPath(Long.toString(user.getId()))
                            .build();
                    final String url = builtUri.toString();

                    PerfilActivity.cargarImagenDeURLenImageView(getContext(),
                            iv_thumbnail, url, LOG_TAG);
                }

                if (tv_nombre != null)
                    tv_nombre.setText(user.getFullName());
                if (tv_trabajo != null)
                    tv_trabajo.setText(user.getTrabajosActuales());    // Revisar si cortar a una línea
                if (tv_recom != null)
                    tv_recom.setText(user.getCantRecomendaciones());
            }
            return itemView;
        }
    }

}
