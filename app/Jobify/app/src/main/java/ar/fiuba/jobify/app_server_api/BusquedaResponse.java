package ar.fiuba.jobify.app_server_api;

import android.support.annotation.Nullable;
import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.shared_server_api.ResponseMetadata;

/**
 * Created by martín on 06/11/16.
 * Estructura API para el response de una búsqueda.
 */
public class BusquedaResponse {

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

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }
}
