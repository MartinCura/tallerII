package ar.fiuba.jobify;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

/**
 * Activity salteada totalmente
  */
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        startActivity(new Intent(this, LoginActivity.class));
//        startActivity(new Intent(this, BusquedaActivity.class));
        //return;

//        setContentView(R.layout.activity_main);
    }
}
