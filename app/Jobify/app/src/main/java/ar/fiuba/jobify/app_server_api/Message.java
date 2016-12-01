package ar.fiuba.jobify.app_server_api;

import android.support.annotation.Nullable;
import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by martín on 31/10/16.
 * Estructura API para un mensaje de una conversación.
 */
public class Message {

    public static String TIMESTAMP_FORMAT = "yyyy-MM-dd HH:mm:ss";

    long  from = 0,
            to = 0;
    String message = "",
         timestamp = TIMESTAMP_FORMAT;


    public Message(long from, long to, String mensaje) {
        this.from = from;
        this.to   = to;
        this.message = mensaje;

        @SuppressWarnings("SimpleDateFormat") //kb
        DateFormat df = new SimpleDateFormat(TIMESTAMP_FORMAT);
        Date dateobj = new Date();
        this.timestamp = df.format(dateobj);
    }

    public long getFrom() {
        return from;
    }

    public long getTo() {
        return to;
    }

    public String getMessage() {
        return message;
    }

    public String getTimestamp() {
        return timestamp;
    }

    public String getTwoLiner() {
        return getFecha() + "\n" + getHora();
    }

    public String getFecha() {
        return timestamp.substring(0, 10);
    }

    public String getHora() {
        return timestamp.substring(11);
    }

    public String getHoraSinSegundos() {
        return getHora().substring(0, 5);
    }

    public Autoria getAutoria(long idPropio) {
        if (this.from == idPropio)
            return Autoria.PROPIO;
        return Autoria.AJENO;
    }

    public enum Autoria {
        AJENO, PROPIO
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
            Log.e("Message", "No se convirtió correctamente, bizarro, ¿culpa de gson?");
            return null;
        }
    }

    @Nullable
    public static Message parseJson(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, Message.class);

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }
}
