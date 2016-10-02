package ar.fiuba.jobify;

import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.support.annotation.IdRes;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
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

import com.android.volley.ParseError;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;

import org.json.JSONException;
import org.json.JSONObject;

import ar.fiuba.jobify.app_server_api.User;

public class NavDrawerActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private final String LOG_TAG = NavDrawerActivity.class.getSimpleName();
    @IdRes int mDrawerResId;

    public long connectedUserID = 0;


    protected void onCreateDrawer(@IdRes int toolbarResId, @IdRes int drawerResId, @IdRes int navResId) {
        mDrawerResId = drawerResId;
        Toolbar toolbar = (Toolbar) findViewById(toolbarResId);
        setSupportActionBar(toolbar);

        SharedPreferences sharedPref = getSharedPreferences(getString(R.string.shared_pref_connected_user), 0);
        connectedUserID = sharedPref.getLong(getString(R.string.stored_connected_user_id), connectedUserID);

        DrawerLayout drawer = (DrawerLayout) findViewById(drawerResId);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close) {
//            public void onDrawerOpened() {}
//            public void onDrawerClosed() {}
        };
        if (drawer != null) {
            drawer.addDrawerListener(toggle);
            toggle.syncState();
        }

        NavigationView navigationView = (NavigationView) findViewById(navResId);
        if (navigationView != null) navigationView.setNavigationItemSelectedListener(this);

        setUpDrawerHeader();
    }

    private void setUpDrawerHeader() {
        setUpDrawerHeaderUser();

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
        }
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
        if (id == R.id.action_settings) {   // TODO: revisar
            startActivity(new Intent(this, SettingsActivity.class));
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.nav_manage) {
            startActivity(new Intent(this, SettingsActivity.class));
            return true;
        } else if (id == R.id.nav_solicitudes) {
            startActivity(
                    new Intent(this, UserListActivity.class)
                            .putExtra(UserListActivity.LIST_MODE, UserListActivity.MODE_SOLICITUDES)
            );
            return true;
        } else if (id == R.id.nav_all_users) {
            startActivity(
                    new Intent(this, UserListActivity.class)
                            .putExtra(UserListActivity.LIST_MODE, UserListActivity.MODE_ALL_USERS)
            );
            return true;
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(mDrawerResId);
        if (drawer != null)
            drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    @Override
    protected void onResume() {
        super.onResume();
        DrawerLayout drawer = (DrawerLayout) findViewById(mDrawerResId);
        if (drawer != null)
            drawer.closeDrawer(GravityCompat.START);
    }

    public void setUpDrawerHeaderUser() {

        Utils.getJsonFromAppServer(this, getString(R.string.perfil_get_user_path), connectedUserID,
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

        ImageView iv_thumbnail = (ImageView) findViewById(R.id.nav_drawer_user_thumbnail);
        Uri builtUri = Uri.parse(Utils.getAppServerBaseURL()).buildUpon()
                .appendPath(getString(R.string.perfil_get_thumbnail_path))
                .appendPath(Long.toString(connectedUserID))
                .build();
        String urlGetThumbnail = builtUri.toString();

        Utils.cargarImagenDeURLenImageView(this, iv_thumbnail, urlGetThumbnail, LOG_TAG);
    }
}