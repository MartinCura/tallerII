package ar.fiuba.jobify.app_server_api;

import android.support.annotation.Nullable;
import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;
import com.google.gson.annotations.SerializedName;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.shared_server_api.ResponseMetadata;

/**
 * Created by martín on 06/11/16.
 * Estructura API para el response de una búsqueda.
 */
public class BusquedaResponse {

    @SerializedName(value="users", alternate={"result", "results"})
    List<User> users;
    ResponseMetadata metadata;


    @SuppressWarnings("unused")
    BusquedaResponse() {
        this.users = new ArrayList<>();
    }

    public List<User> getUsers() {
        return users;
    }

    @Nullable
    public ResponseMetadata getMetadata() {
        return metadata;
    }

    @Nullable
    public static BusquedaResponse parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, BusquedaResponse.class);

//            try {
//                JSONArray jsonArray = new JSONObject(response).getJSONArray("result");
//
//                // No es lo más lindo pero es lo más rápido que se me ocurrió.
//                int i = 0;
//                for (User user : br.users) {
//                    user.addEmployment(gson.fromJson(
//                            jsonArray.getJSONObject(i).getJSONObject("current_job").toString(), // hardcodeo
//                            Employment.class)
//                    );
//                    i++;
//                }
//            } catch (JSONException ex) {
//                Log.w("BusquedaResponse", "JSON Exception!!");
//            }
//
//            return br;

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }
}
