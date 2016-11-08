package ar.fiuba.jobify;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.LayoutRes;
import android.support.v7.app.ActionBar;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.app_server_api.BusquedaRequest;
import ar.fiuba.jobify.shared_server_api.JobPosition;
import ar.fiuba.jobify.shared_server_api.SharedDataSingleton;
import ar.fiuba.jobify.shared_server_api.Skill;

public class BusquedaActivity extends NavDrawerActivity {

    private final String LOG_TAG = BusquedaActivity.class.getSimpleName();

    private String mSelectedJobPositionString = "";
    private String mSelectedSkillString = "";
    private EditableListAdapter<Skill> mSkillAdapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_busqueda_drawer);

        ActionBar sab = getSupportActionBar();
        if (sab != null) sab.setDisplayHomeAsUpEnabled(true);

        populateSpinners();

        mSkillAdapter = EditableListAdapter.populateEditableList(this,
                (ListView) findViewById(R.id.skill_list),
                new ArrayList<Skill>()
        );
    }

    @Override
    public void setContentView(@LayoutRes int layoutResID) {
        super.setContentView(layoutResID);
        onCreateDrawer(R.id.busqueda_toolbar, R.id.busqueda_drawer_layout, R.id.busqueda_nav_view);
    }

    @Override
    public void onResume() {
        super.onResume();

        populateSpinners(); // TODO: Revisar que no se acumule
    }


    private void populateSpinners() {

        final Context ctx = this;//

        try {
            Spinner spinner = (Spinner) findViewById(R.id.job_positions_spinner);
            if (spinner == null) {
                Log.e(LOG_TAG, "Spinner de job positions no encontrado!");
                return;
            }
            List<JobPosition> jobPositions = SharedDataSingleton.getInstance(this).getJobPositions();
            if (jobPositions != null) {

                ArrayList<String> jpArray = new ArrayList<>();
                for (JobPosition jp : jobPositions) {
                    jpArray.add(jp.getNombre());
                }
                ArrayAdapter<String> jpAdapter =
                        new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, jpArray);
                jpAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                spinner.setAdapter(jpAdapter);

                spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                    @Override
                    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
                        mSelectedJobPositionString = (String) parent.getItemAtPosition(pos);
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parent) {
                        Toast.makeText(ctx, "onNothingSelected()", Toast.LENGTH_LONG)
                                .show();//
                        mSelectedJobPositionString = "";
                    }
                });
            }
        } catch (SharedDataSingleton.NoDataException ex) {
            Toast.makeText(this, "Problemas con SS.JobPositions", Toast.LENGTH_LONG)
                    .show();//
            Log.e(LOG_TAG, "Problemas con SS.JobPositions");
        }

        try {
            Spinner spinner = (Spinner) findViewById(R.id.skills_spinner);
            if (spinner == null) {
                Log.e(LOG_TAG, "Spinner de skills no encontrado!");
                return;
            }
            List<Skill> skills = SharedDataSingleton.getInstance(this).getSkills();
            if (skills != null) {

                ArrayList<String> skArray = new ArrayList<>();
                for (Skill sk : skills) {
                    skArray.add(sk.getNombre());
                }
                ArrayAdapter<String> skAdapter =
                        new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, skArray);
                skAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                spinner.setAdapter(skAdapter);

                spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                    @Override
                    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
                        mSelectedSkillString = (String) parent.getItemAtPosition(pos);
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parent) {
                        Toast.makeText(ctx, "onNothingSelected()", Toast.LENGTH_LONG)
                                .show();//
                        mSelectedSkillString = "";
                    }
                });
            }
        } catch (SharedDataSingleton.NoDataException ex) {
            Toast.makeText(this, "Problemas con SS.Skills", Toast.LENGTH_LONG)
                    .show();//
            Log.e(LOG_TAG, "Problemas con SS.Skills");
        }
    }

    public void agregarSkill(View v) {
        Spinner spinner = (Spinner) findViewById(R.id.skills_spinner);
        if (spinner == null) {
            Log.e(LOG_TAG, "Spinner de skills no encontrado!");
            return;
        }
        if (mSelectedSkillString.isEmpty()) return;

        try {
            Skill nuevoSkill = Skill.create(this, mSelectedSkillString);
            if (nuevoSkill == null) return;// false;

            if (!mSkillAdapter.add(nuevoSkill, true)) {
                Toast.makeText(this, "Skill ya listado", Toast.LENGTH_LONG)
                        .show();
                return;// false;
            }
            mSkillAdapter.notifyDataSetChanged();

        } catch (IllegalArgumentException ex) {
            Log.e(LOG_TAG, "How...???.....");
            //return false;
        }
        //return true;
    }

    public void comenzarBusqueda(View v) {
        Toast.makeText(this, "Buscando...", Toast.LENGTH_LONG)
                .show();//

        int distancia = Utils.getTextViewInt(this, R.id.busqueda_distancia);
        if (distancia < 0) distancia = 0;

        BusquedaRequest busquedaReq = BusquedaRequest.crear(mSelectedJobPositionString,
                                                        mSkillAdapter.getList(), distancia);
        Log.d(LOG_TAG, "BusqRequest: "+busquedaReq.toJson());//
        startActivity(
                new Intent(this, UserListActivity.class)
                        .putExtra(UserListActivity.LIST_MODE, UserListActivity.MODE_BUSQUEDA)
                        .putExtra(UserListActivity.BUSQUEDA_REQUEST_MESSAGE, busquedaReq.toJson())
        );
    }
}
