package ar.fiuba.jobify;

import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.annotation.IdRes;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBar;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.Response;

import org.json.JSONObject;

import java.util.ArrayList;

import ar.fiuba.jobify.app_server_api.Contact;
import ar.fiuba.jobify.app_server_api.ContactsResponse;
import ar.fiuba.jobify.app_server_api.ConversationsResponse;
import ar.fiuba.jobify.app_server_api.User;
import ar.fiuba.jobify.utils.Utils;

public class NavDrawerActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private final String LOG_TAG = NavDrawerActivity.class.getSimpleName();
    @IdRes int mDrawerResId;

    public long connectedUserID = 0;


    protected void onCreateDrawer(@IdRes int toolbarResId, @IdRes int drawerResId, @IdRes int navResId) {
        mDrawerResId = drawerResId;
        Toolbar toolbar = (Toolbar) findViewById(toolbarResId);
        if (toolbar == null) {
            Log.e(LOG_TAG, "Toolbar no encontrado");
        }
        setSupportActionBar(toolbar);

        SharedPreferences sharedPref = getSharedPreferences(getString(R.string.shared_pref_connected_user), 0);
        connectedUserID = sharedPref.getLong(getString(R.string.stored_connected_user_id), connectedUserID);

        DrawerLayout drawer = (DrawerLayout) findViewById(drawerResId);
        ActionBarDrawerToggle toggle;
        toggle = new ActionBarDrawerToggle(this, drawer, toolbar,
                R.string.navigation_drawer_open, R.string.navigation_drawer_close) {
//                        public void onDrawerOpened() {}
//                        public void onDrawerClosed() {}
        };
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setHomeButtonEnabled(true);
        }
        if (drawer == null)
            Log.e(LOG_TAG, "SYSTEM FAILURE SYSTEM FAILURE SYSTEM FAILURE SYSTEM FAILURE ...");
        if (drawer != null) {
            drawer.addDrawerListener(toggle);
            toggle.syncState();
        }

        NavigationView navigationView = (NavigationView) findViewById(navResId);
        if (navigationView != null) {
            navigationView.setNavigationItemSelectedListener(this);
            setUpNavigationMenu(navigationView);
        } else {
            Log.e(LOG_TAG, "Nav View no encontrado!");
        }

//        setUpDrawerHeader();
    }

    @Override
    protected void onResume() {
        super.onResume();

        setUpDrawerHeader();
    }

    private void setUpDrawerHeader() {
        // Misterio de la naturaleza
        LinearLayout headerLayout = (LinearLayout) findViewById(R.id.nav_drawer_header_layout);
        if (headerLayout != null) {
            headerLayout.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (connectedUserID != 0) {
                        startActivity(
                                new Intent(NavDrawerActivity.this, PerfilActivity.class)
                                        .putExtra(PerfilActivity.FETCHED_USER_ID_MESSAGE,
                                                connectedUserID)
                        );
                    }
                }
            });
//        } else {
//            Log.d(LOG_TAG, "No puedo encontrar el headerLayout... :(");
        }

        setUpDrawerHeaderUser();
    }

    public void setUpDrawerHeaderUser() {
        Utils.getJsonFromAppServer(this, getString(R.string.get_user_path), connectedUserID,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        User mUser = User.parseJSON(response.toString());
                        if (mUser != null) {
                            fillDrawerHeaderText(mUser);

                        } else {
                            Log.e(LOG_TAG, "Error de parseo de usuario, no puedo llenar el header del ND");
                        }
                    }
                }, LOG_TAG);

        String urlGetThumbnail = Utils.getAppServerUrl(this, connectedUserID, getString(R.string.get_thumbnail_path));
        ImageView iv_thumbnail = (ImageView) findViewById(R.id.nav_drawer_user_thumbnail);

        if (iv_thumbnail != null)
            Utils.cargarImagenDeURLenImageView(this, iv_thumbnail, urlGetThumbnail, LOG_TAG, true);
    }

    private void fillDrawerHeaderText(User user) {
        TextView tv_nombre = (TextView) findViewById(R.id.nav_drawer_user_nombre);
        if (tv_nombre != null)
            tv_nombre.setText(user.getFullName());

        TextView tv_mail = (TextView) findViewById(R.id.nav_drawer_user_mail);
        if (tv_mail != null)
            tv_mail.setText(user.getEmail());
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(mDrawerResId);
        if (drawer != null && drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.nav_drawer, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            startActivity(new Intent(this, SettingsActivity.class));
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void setUpNavigationMenu(NavigationView navView) {
        final Menu navMenu = navView.getMenu();
        final MenuItem solicitudesItem = navMenu.findItem(R.id.nav_solicitudes);
        final MenuItem conversacionesItem = navMenu.findItem(R.id.nav_conversaciones);

        String urlContactos = Utils.getAppServerUrl(this, connectedUserID, getString(R.string.get_contacts_path));
        Utils.fetchJsonFromUrl(this, Request.Method.GET, urlContactos, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        ContactsResponse contactsResponse =
                                ContactsResponse.parseJSON(response.toString());

                        if (contactsResponse == null) {
                            Log.e(LOG_TAG, "ContactsResponse null");
                        } else {
                            ArrayList<Contact> contactsReceived =
                                    contactsResponse.getContactsWithStatus(Contact.Status.RECEIVED);
                            int cantSolicitudes = contactsReceived.size();
                            if (cantSolicitudes > 0) {
                                String newTitle = getString(R.string.nav_solicitudes_option)
                                        + " (" + cantSolicitudes + ")";
                                solicitudesItem.setTitle(newTitle);
                            } else {
                                navMenu.removeItem(R.id.nav_solicitudes);
                            }
                        }
                    }
                }, LOG_TAG);

        String urlConversaciones = Utils.getAppServerUrl(this, connectedUserID, getString(R.string.get_conversations_path));
        Utils.fetchJsonFromUrl(this, Request.Method.GET, urlConversaciones, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        ConversationsResponse convResponse =
                                ConversationsResponse.parseJSON(response.toString());

                        if (convResponse == null) {
                            Log.e(LOG_TAG, "ConversationsResponse null");
                        } else {
                            long cantUnread = convResponse.getMetadata().getTotalCount();
                            if (cantUnread > 0) {
                                String newTitle = getString(R.string.nav_conversations_option)
                                        + " (" + cantUnread + ")";
                                conversacionesItem.setTitle(newTitle);
                                conversacionesItem.setIcon(R.drawable.ic_conversaciones_cerrado);
                            }
                        }
                    }
                }, LOG_TAG);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        DrawerLayout drawer = (DrawerLayout) findViewById(mDrawerResId);
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.nav_manage) {
            if (drawer != null)
                drawer.closeDrawer(GravityCompat.START);
            startActivity(
                    new Intent(this, SettingsActivity.class)
            );
            return false;

        } else if (id == R.id.nav_solicitudes) {
            if (drawer != null)
                drawer.closeDrawer(GravityCompat.START);
            startActivity(
                    new Intent(this, UserListActivity.class)
                            .putExtra(UserListActivity.LIST_MODE_MESSAGE,
                                      UserListActivity.MODE_SOLICITUDES)
            );
            return false;

        } else if (id == R.id.nav_most_popular) {
            if (drawer != null)
                drawer.closeDrawer(GravityCompat.START);
            startActivity(
                    new Intent(this, UserListActivity.class)
                            .putExtra(UserListActivity.LIST_MODE_MESSAGE,
                                      UserListActivity.MODE_MOST_POPULAR)
            );
            return false;

        } else if (id == R.id.nav_busqueda) {
            if (drawer != null)
                drawer.closeDrawer(GravityCompat.START);
            startActivity(
                    new Intent(this, BusquedaActivity.class)
            );
            return false;

        } else if (id == R.id.nav_conversaciones) {
            if (drawer != null)
                drawer.closeDrawer(GravityCompat.START);
            startActivity(
                    new Intent(this, UserListActivity.class)
                            .putExtra(UserListActivity.LIST_MODE_MESSAGE,
                                      UserListActivity.MODE_CONVERSACIONES)
            );
            return false;

        } else if (id == R.id.nav_logout) {
            Utils.confirmarAccion(this, "Cerrar sesión",
                    "¿Está seguro de que quiere cerrar sesión?\nLo extrañaremos mucho...",
                    new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            borrarConnectedUserData();
                            finishAffinity();
                        }
                    });
            return false;
        }

        return true;
    }

    protected void bloquearNavDrawer(boolean block) {
        DrawerLayout drawer = (DrawerLayout) findViewById(mDrawerResId);
        if (drawer == null) {
            Log.e(LOG_TAG, "No encontré drawer para bloquearlo!");
            return;
        }
        if (block) {
            drawer.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED);
        } else {
            drawer.setDrawerLockMode(DrawerLayout.LOCK_MODE_UNDEFINED);
        }
    }

    public void irAPerfilPropio(View v) {
        Utils.iniciarPerfilActivity(this, connectedUserID, false);
    }

    private void borrarConnectedUserData() {
        SharedPreferences.Editor editor = getSharedPreferences(getString(R.string.shared_pref_connected_user), 0).edit();
        editor.remove(getString(R.string.stored_connected_user_id));
        editor.remove(getString(R.string.stored_connected_user_token));
        editor.apply();
    }
}