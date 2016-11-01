package ar.fiuba.jobify.app_server_api;

import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by martín on 31/10/16.
 * Estructura API para un mensaje de una conversación.
 */
public class Message {

    long  from = 0,
            to = 0;
    String message = "",
         timestamp = "AAAA-MM-DD HH:mm:SS";

    public Message(long from, long to, String mensaje) {
        this.from = from;
        this.to   = to;
        this.message = mensaje;
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
        // TODO
        return timestamp.substring(0, 10);
//        return timestamp;
    }

    public String getHora() {
        // TODO
        return timestamp.substring(11);//
//        return timestamp;
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
}
