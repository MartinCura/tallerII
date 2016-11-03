package ar.fiuba.jobify.app_server_api;

import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;

/**
 * Created by martín on 30/10/16.
 * Estructura API para crear y cancelar recomendaciones de un usuario a otro.
 */
public class Recommendation {

    long  from = 0,
            to = 0;

    public long getFrom() {
        return from;
    }

    public long getTo() {
        return to;
    }

    public Recommendation(long recomendador, long recomendado) {
        this.from = recomendador;
        this.to   = recomendado;
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
            Log.e("Recommendation", "No se convirtió correctamente, bizarro, ¿culpa de gson?");
            return null;
        }
    }

    public HashMap<String, String> toQueries() {
        HashMap<String, String> map = new HashMap<>();
        map.put("from", Long.toString(from));
        map.put("to",   Long.toString(to));
        return map;
    }
}
