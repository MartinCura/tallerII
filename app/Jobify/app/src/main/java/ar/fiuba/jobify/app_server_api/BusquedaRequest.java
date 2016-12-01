package ar.fiuba.jobify.app_server_api;

import android.content.Context;
import android.support.annotation.Nullable;
import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import ar.fiuba.jobify.R;
import ar.fiuba.jobify.shared_server_api.Skill;
import ar.fiuba.jobify.utils.Utils;

/**
 * Created by martín on 02/11/16.
 * Estructura API para obtener una búsqueda.
 * NO se envía directamente como en otros casos; utilizar {@code generarRequestUrl()}.
 */
public class BusquedaRequest {

    String name = null;
    String mail = null;
    String jobPosition = null;
    List<String> skills;
    int distancia = 0;


    public BusquedaRequest() {
        skills = new ArrayList<>();
    }

    public static BusquedaRequest crear(String nombre, String email, String nombreJobPosition,
                                        List<String> skills, int distancia) {
        BusquedaRequest br = new BusquedaRequest();

        br.name = (nombre == null) ? null : nombre.toLowerCase();
        br.mail = (email == null) ? null : email.toLowerCase();
        br.jobPosition = nombreJobPosition != null && !nombreJobPosition.isEmpty()
                ? nombreJobPosition.toLowerCase() : null;
        if (skills != null) {
            for (String sk : skills) {
                if (sk != null)
                    br.skills.add(sk);
            }
        }
//        for (Skill sk : skills) {
//            if (sk != null && sk.getName() != null)
//                br.skills.add(sk.getName().toLowerCase());
//        }
        br.distancia = distancia;

        return br;
    }


    // pageNumber empieza en 1
    public String generarRequestUrl(Context ctx, int pageSize, int pageNumber, Locacion loc) {
        HashMap<String, String> map = new HashMap<>();

        if (pageNumber < 1)
            pageNumber = 1;
        map.put(ctx.getString(R.string.get_busqueda_users_page_number_query), Integer.toString(pageNumber));
        if (pageSize < 1)
            pageSize = 1;
        map.put(ctx.getString(R.string.get_busqueda_users_page_size_query),  Integer.toString(pageSize));

        if (name != null && !name.isEmpty())
            map.put(ctx.getString(R.string.get_busqueda_users_name_query), name);
        if (mail != null && !mail.isEmpty())
            map.put(ctx.getString(R.string.get_busqueda_users_mail_query), mail);

        if (jobPosition != null && !jobPosition.isEmpty())
            map.put(ctx.getString(R.string.get_busqueda_users_job_positions_query), jobPosition);
        if (skills != null) {
            String sks = "";
            for (String sk : skills) {
                if (sk != null && !sk.isEmpty()) {
                    if (!sks.isEmpty())
                        sks += ",";
                    sks += sk;
                }
            }
            if (!sks.isEmpty())
                map.put(ctx.getString(R.string.get_busqueda_users_skills_query), sks);
        }
        if (distancia > 0 && loc != null) {
            String d = loc.getLatitude() + "," + loc.getLongitude() + "," + Integer.toString(distancia);
            map.put(ctx.getString(R.string.get_busqueda_users_distance_query), d);
        }
        return Utils.getAppServerUrl(ctx, ctx.getString(R.string.get_search_path), map);
    }

    public boolean incluyeDistancia() {
        return distancia > 0;
    }


    public String toJson() {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        return gson.toJson(this);
    }

//    public JSONObject toJsonObject() {
//        try {
//            return new JSONObject(toJson());
//
//        } catch (JSONException ex) {
//            ex.printStackTrace();
//            Log.e("BusquedaRequest", "No se convirtió correctamente, bizarro, ¿culpa de gson?");
//            return null;
//        }
//    }

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
