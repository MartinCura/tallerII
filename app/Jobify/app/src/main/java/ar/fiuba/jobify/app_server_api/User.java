package ar.fiuba.jobify.app_server_api;

import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.shared_server_api.Skill;

/**
 * Created by martín on 08/09/16.
 */
public class User {

    int id;
    String  firstName = "",
            lastName = "",
            email = "",
            city = "",
            dateOfBirth = "",
            summary = "";
    String profilePicture; // URL? TODO revisar
    List<Skill> skills;
    List<Employment> workHistory;


    public User() {
        skills = new ArrayList<>();
        workHistory = new ArrayList<>();
    }

    public int getId() {
        return id;
    }
    public String getFirstName() {
        return firstName;
    }
    public String getLastName() {
        return lastName;
    }
    public String getFullName() {
        return getFirstName()+" "+getLastName();
    }
    public String getEmail() {
        return email;
    }
    public String getCity() {
        return city;
    }
    public String getDateOfBirth() {
        return dateOfBirth;
    }
    public String getSummary() {
        return summary;
    }
    public String getProfilePicture() {
        return profilePicture;
    }
    public List<Skill> getSkills() {
        return skills;
    }
    public List<Employment> getWorkHistory() {
        return workHistory;
    }

    public int getCantRecomendaciones() {   // TODO
        return 123;
        //return cantRecomendaciones;
    }

    /**
     * @return String de una o varias líneas con los trabajos actuales,
     * determinado por {@code Employment.esActual}.
     */
    public String getTrabajoActual() {
        String actual = "";
        for (Employment trabajo : workHistory) {
            if (trabajo.esActual()) {
                if (!actual.equals(""))
                    actual = actual.concat("\n");
                actual = actual.concat(trabajo.getOneLiner());
            }
        }
        return actual;
    }

    /**
     * @return String del formato {@code Fecha de nacimiento: 01/01/1990}.
     */
    public String getLineaNacimiento() {
        return "Fecha de nacimiento: " + getDateOfBirth();
    }

    // Temporal TODO
    public List<String> getListaJobs() {
        List<String> lista = new ArrayList<>();
        for (Employment job : getWorkHistory()) {
            lista.add(job.getOneLiner());
        }
        return lista;
    }

    // Temporal TODO
    public List<String> getListaSkills() {
        List<String> lista = new ArrayList<>();
        for (Skill skill : getSkills()) {
            lista.add(skill.getName());
        }
        return lista;
    }

    public static User parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            User user = gson.fromJson(response, User.class);
            return user;

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }
}
