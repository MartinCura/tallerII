package ar.fiuba.jobify.app_server_api;

import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.shared_server_api.ResponseMetadata;

/**
 * Created by martín on 01/10/16.
 * Estructura API para un GET de los contactos de un usuario.
 */
public class ContactsResponse {

    List<Contact> contacts;
    ResponseMetadata metadata;

    public ContactsResponse() {
        contacts = new ArrayList<>();
    }

    public static ContactsResponse parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, ContactsResponse.class);

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }

    public List<Contact> getContacts() {
        return contacts;
    }

    public ResponseMetadata getMetadata() {
        return metadata;
    }


    public Contact.Status getStatusForId(long idRequester) {
        for ( Contact c : contacts ) {
            if (c.getId() == idRequester)
                return c.getStatus();
        }
        return Contact.Status.NONE;
    }

    public ArrayList<Contact> getContactsWithStatus(Contact.Status st) {
        ArrayList<Contact> list = new ArrayList<>();
        for (Contact c : contacts) {
            if (c.getStatus().equals(st))
                list.add(c);
        }
        return list;
    }
}
