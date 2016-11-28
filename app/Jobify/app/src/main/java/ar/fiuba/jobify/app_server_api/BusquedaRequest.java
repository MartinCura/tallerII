package ar.fiuba.jobify.app_server_api;

import android.support.annotation.Nullable;
import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.shared_server_api.Skill;

/**
 * Created by martín on 02/11/16.
 * Estructura API para obtener una búsqueda.
 */
public class BusquedaRequest {

    String jobPosition = null;
    List<String> skills;
//    JobPosition jobPosition;   // TODO: TENDRÍA QUE ENVIAR ESTO? Mirar todo lo comentado si cambia
//    List<Skill> skills;
    int distancia = 0;


    public BusquedaRequest() {
        skills = new ArrayList<>();
    }

    public static BusquedaRequest crear(String nombreJobPosition, List<Skill> skills,
                                        int distancia/*, Activity activity*/) {
        BusquedaRequest br = new BusquedaRequest();

        br.jobPosition = !nombreJobPosition.isEmpty() ? nombreJobPosition : null;
//        br.jobPosition = JobPosition.create(activity, nombreJobPosition);
//        int i = 0;
        for (Skill sk : skills) {
            if (sk != null)
                br.skills.add(sk.getName());
//            i++;
        }
//        br.skills.addAll(skills);
        br.distancia = distancia;

        return br;
    }


    public String toJson() {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        return gson.toJson(this);
    }

    public JSONObject toJsonObject() {
        try {
            return new JSONObject(toJson());

        } catch (JSONException ex) {
            ex.printStackTrace();
            Log.e("BusquedaRequest", "No se convirtió correctamente, bizarro, ¿culpa de gson?");
            return null;
        }
    }

    @Nullable
    public static BusquedaRequest parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, BusquedaRequest.class);

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }
}
