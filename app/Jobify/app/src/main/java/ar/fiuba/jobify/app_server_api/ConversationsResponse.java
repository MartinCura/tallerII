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
 * Created by martín on 18/11/16.
 * Estructura API para recibir la lista de conversaciones iniciadas.
 */
public class ConversationsResponse {

    /** Guardo los corresponsales en Users,
     * metiendo la cantidad de unread en la cantidadRecomendaciones **/
    List<User> conversations;
    ResponseMetadata metadata;

    ConversationsResponse() {
        this.conversations = new ArrayList<>();
    }

    public List<User> getConversations() {
        return conversations;
    }

    public ResponseMetadata getMetadata() {
        return metadata;
    }

    @Nullable
    public static ConversationsResponse parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, ConversationsResponse.class);

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }
}
