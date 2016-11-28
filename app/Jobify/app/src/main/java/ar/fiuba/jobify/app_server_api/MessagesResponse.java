package ar.fiuba.jobify.app_server_api;

import android.support.annotation.Nullable;
import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import ar.fiuba.jobify.shared_server_api.ResponseMetadata;

/**
 * Created by martín on 31/10/16.
 * Estructura API para recibir varios mensajes.
 */
public class MessagesResponse {

    List<Message> messages;
    ResponseMetadata metadata;

    MessagesResponse() {
        messages = new ArrayList<>();
    }

    public List<Message> getMessages() {
        return messages;
    }

    @Nullable
    public ResponseMetadata getMetadata() {
        return metadata;
    }

    /** Ordena de mayor a menor */
    MessagesResponse sort() {
        Collections.sort(this.messages, new Comparator<Message>() {
            @Override
            public int compare(Message m1, Message m2) {
                return m1.getTimestamp().compareTo(m2.getTimestamp());
            }
        });
        return this;
    }

    /** Ordena de mayor a menor antes de devolver */
    @Nullable
    public static MessagesResponse parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, MessagesResponse.class).sort();

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }

}
