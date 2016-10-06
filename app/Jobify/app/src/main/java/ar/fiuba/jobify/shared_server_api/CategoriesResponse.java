package ar.fiuba.jobify.shared_server_api;

import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by martín on 06/09/16.
 * Estructura API para las categorías del sistema.
 */
public class CategoriesResponse {

    List<Category> categories;
    ResponseMetadata metadata;


    public CategoriesResponse() {
        categories = new ArrayList<>();
    }

    public static CategoriesResponse parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, CategoriesResponse.class);

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }

    public List<Category> getCategories() {
        return categories;
    }

    public ResponseMetadata getMetadata() {
        return metadata;
    }
}
