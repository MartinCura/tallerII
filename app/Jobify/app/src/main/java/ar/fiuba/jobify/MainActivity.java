package ar.fiuba.jobify;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.net.Uri;

public class MainActivity extends AppCompatActivity
    implements PerfilFragment.OnFragmentInteractionListener {

    private final String LOG_TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        startActivity(new Intent(this, PerfilActivity.class));
        return;

//        setContentView(R.layout.activity_main);
//
//        if (savedInstanceState == null) {
//            getSupportFragmentManager().beginTransaction()
//                    .add(R.id.container, new PerfilFragment())
//                    .commit();
//        }
    }

    public void onFragmentInteraction(Uri uri) {

    }
}

/** Código útil */
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();