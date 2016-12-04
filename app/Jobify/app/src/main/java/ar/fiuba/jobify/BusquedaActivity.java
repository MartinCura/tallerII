package ar.fiuba.jobify;

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
import ar.fiuba.jobify.utils.EditableListAdapter;
import ar.fiuba.jobify.utils.Utils;

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

        ListView skillsListView = (ListView) findViewById(R.id.skill_list);
        if (skillsListView != null) {
            mSkillAdapter = EditableListAdapter.populateEditableList(this,
                    skillsListView,
                    new ArrayList<Skill>(),
                    true
            );
        }
    }

    @Override
    public void setContentView(@LayoutRes int layoutResID) {
        super.setContentView(layoutResID);
        onCreateDrawer(R.id.busqueda_toolbar, R.id.busqueda_drawer_layout, R.id.busqueda_nav_view);
        displayItemAsSelected(R.id.nav_busqueda);
    }

    @Override
    public void onResume() {
        super.onResume();

        populateSpinners();
    }


    private void populateSpinners() {
        final String opcionVaciaStr = "(opcional)";
        try {
            Spinner spinner = (Spinner) findViewById(R.id.job_positions_spinner);
            if (spinner == null) {
                Log.e(LOG_TAG, "Spinner de job positions no encontrado!");
                return;
            }
            List<JobPosition> jobPositions = SharedDataSingleton.getInstance(this).getJobPositions();
            if (jobPositions != null) {

                ArrayList<String> jpArray = new ArrayList<>();
                jpArray.add(opcionVaciaStr);
                for (JobPosition jp : jobPositions) {
                    if (jp != null)
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
                        if (mSelectedJobPositionString.equals(opcionVaciaStr))
                            mSelectedJobPositionString = "";
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parent) {
                        mSelectedJobPositionString = "";
                    }
                });
            }
        } catch (SharedDataSingleton.NoDataException ex) {
//            Toast.makeText(this, "Problemas con SS.JobPositions", Toast.LENGTH_LONG)
//                    .show();//
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
                skArray.add(opcionVaciaStr);
                for (Skill sk : skills) {
                    if (sk != null)
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
                        if (mSelectedSkillString.equals(opcionVaciaStr))
                            mSelectedSkillString = "";
                    }

                    @Override
                    public void onNothingSelected(AdapterView<?> parent) {
                        mSelectedSkillString = "";
                    }
                });
            }
        } catch (SharedDataSingleton.NoDataException ex) {
//            Toast.makeText(this, "Problemas con SS.Skills", Toast.LENGTH_LONG)
//                    .show();//
            Log.e(LOG_TAG, "Problemas con SS.Skills");
        }
    }

    public void agregarSkill(View v) {
        Spinner spinner = (Spinner) findViewById(R.id.skills_spinner);
        if (spinner == null) {
            Log.e(LOG_TAG, "Spinner de skills no encontrado!");
            return;
        }
        if (mSelectedSkillString.isEmpty())
            return;

        try {
            Skill nuevoSkill = Skill.create(this, mSelectedSkillString);
            if (nuevoSkill == null)
                return;

            if (!mSkillAdapter.add(nuevoSkill, true)) {
                Toast.makeText(this, "Skill ya listado", Toast.LENGTH_LONG)
                        .show();
                return;
            }
            mSkillAdapter.notifyDataSetChanged();

        } catch (IllegalArgumentException ex) {
            Log.e(LOG_TAG, "How...???.....");
        }
    }

    public void comenzarBusqueda(View v) {
        int distancia = Utils.getTextViewInt(this, R.id.busqueda_distancia);
        if (distancia < 0) distancia = 0;

        String nombre = Utils.getTextViewText(this, R.id.busqueda_nombre);
        if (nombre.isEmpty()) nombre = null;

        String mail = Utils.getTextViewText(this, R.id.busqueda_mail);
        if (mail.isEmpty()) mail = null;

        BusquedaRequest busquedaReq = BusquedaRequest.crear(nombre, mail,
                mSelectedJobPositionString, mSkillAdapter.getStringList(), distancia);
        Log.d(LOG_TAG, "BusqRequest: "+busquedaReq.toJson());//
        startActivity(
                new Intent(this, UserListActivity.class)
                        .putExtra(UserListActivity.LIST_MODE_MESSAGE, UserListActivity.MODE_BUSQUEDA)
                        .putExtra(UserListActivity.BUSQUEDA_REQUEST_MESSAGE, busquedaReq.toJson())
        );
    }

    @Override
    protected void iniciarBusqueda() {
        // Ya estoy en activity de búsqueda
    }
}
