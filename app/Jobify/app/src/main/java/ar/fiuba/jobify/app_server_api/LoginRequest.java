package ar.fiuba.jobify.app_server_api;

import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by martín on 04/10/16.
 * Estructura API para pedido de login/registro.
 */
public class LoginRequest {

    // TODO: Revisar manera en que se guarda la contraseña
    String email;
    String password;

    // Acá podría validarse algo más si hiciera falta

    public LoginRequest(String email, String password) {
        setEmail(email);
        setPassword(password);
    }

    private void setEmail(String email) {
        this.email = email.toLowerCase();
    }

    private void setPassword(String password) {
        // TODO: Hashear?
        this.password = password;
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
            Log.e("LoginRequest", "No se convirtió correctamente, bizarro, ¿culpa de gson?");
            return null;
        }
    }
}
