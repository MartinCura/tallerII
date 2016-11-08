package ar.fiuba.jobify;

import com.google.gson.JsonObject;

import org.json.JSONObject;

/**
 * Created by martin on 11/3/16.
 */
public class MessageEvent {
    public JSONObject mensaje;

    MessageEvent (JSONObject mensaje){
        this.mensaje = mensaje;
    }
}
