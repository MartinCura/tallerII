package ar.fiuba.jobify.app_server_api;

import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.annotations.SerializedName;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by martín on 01/10/16.
 * Estructura de una solicitud de contacto como debe ser enviada al AppServer.
 */
public class Solicitud {

    long    authorId = 0,
            contactId = 0;
    Action  action;

    public Solicitud(long idAutor, long idContacto, Action status) {
        this.authorId = idAutor;
        this.contactId = idContacto;
        this.action = status;
    }

    public long getAuthorId() {
        return authorId;
    }

    public long getContactId() {
        return contactId;
    }

    public Action getAction() {
        return action;
    }


    public enum Action {

        @SerializedName("remove_contact")
        UNFRIEND (0),

        @SerializedName("add_contact")
        ADD (1),

        @SerializedName("accept_contact")
        ACCEPT (2);

        private final int value;

        Action(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

    }

    // TODO: extraer todos estos a una clase padre de todas
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
            Log.e("Solicitud", "No se convirtió correctamente, bizarro, ¿culpa de gson?");
            return null;
        }
    }
}
