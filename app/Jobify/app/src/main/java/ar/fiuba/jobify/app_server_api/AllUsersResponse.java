package ar.fiuba.jobify.app_server_api;

import android.support.annotation.Nullable;
import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import ar.fiuba.jobify.shared_server_api.ResponseMetadata;

/**
 * Created by martín on 04/10/16.
 * Estructura API para un GET de todos los usuarios.
 */
public class AllUsersResponse {

    long[] allUsers;
    ResponseMetadata metadata;


    @Nullable
    public static AllUsersResponse parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, AllUsersResponse.class);

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }

    public long[] getAllUsers() {
        return allUsers;
    }

    @Nullable
    public ResponseMetadata getMetadata() {
        return metadata;
    }
}
