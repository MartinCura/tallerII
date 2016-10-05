package ar.fiuba.jobify.app_server_api;

import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.shared_server_api.Skill;

/**
 * Created by martín on 08/09/16.
 * Clase que maneja todos los elementos de un usuario del sistema.
 * Para la creación de un nuevo usuario, usar el constructor de e-mail.
 * La única otra forma permitida por el momento de crear uno entero es a partir de un Json
 * de esa forma evitando que se toque el id, elemento decidido únicamente por el servidor.
 */
public class User {

    public final static int MAX_CHAR_NAMES = 35;
    public final static int MAX_CHAR_SUMMARY = 1000;

    long id;
    String  firstName = "",
            lastName = "",
            email = "",
            city = "",
            dateOfBirth = "",
            summary = "";
    List<Skill> skills;
    List<Employment> workHistory;


    public User() {
        skills = new ArrayList<>();
        workHistory = new ArrayList<>();
    }

    public User(String email) {
        this();
        this.email = email;
    }

    // copy constructor
    public User(User o) {
        this.id = o.id;
        this.firstName = o.firstName;
        this.lastName = o.lastName;
        this.email = o.email;
        this.city = o.city;
        this.dateOfBirth = o.dateOfBirth;
        this.summary = o.summary;
        this.skills = new ArrayList<>(o.skills);
        this.workHistory = new ArrayList<>(o.workHistory);
    }

    public long getId() {
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
    public List<Skill> getSkills() {
        return skills;
    }
    public List<Employment> getWorkHistory() {
        return workHistory;
    }

    public long getCantRecomendaciones() {   // TODO
        return 123;
        //return cantRecomendaciones;
    }

    public boolean setFirstName(String firstName) {
        if (firstName.isEmpty() || firstName.length() > MAX_CHAR_NAMES)
            return false;
        this.firstName = firstName;
        return true;
    }

    public boolean setLastName(String lastName) {
        if (lastName.isEmpty() || lastName.length() > MAX_CHAR_NAMES)
            return false;
        this.lastName = lastName;
        return true;
    }

    public boolean setCity(String city) {
        // if?? TODO
        this.city = city;
        return true;
    }

    public boolean setDateOfBirth(String dateOfBirth) {
        // TODO if
        this.dateOfBirth = dateOfBirth;
        return true;
    }

    public boolean setSummary(String summary) {
        if (summary.length() > MAX_CHAR_SUMMARY)
            return false;
        this.summary = summary;
        return true;
    }

    public void setSkills(List<Skill> skills) {
        // Chequeos?? TODO
        this.skills = new ArrayList<>(skills);
    }

    public void setWorkHistory(List<Employment> workHistory) {
        // Chequeos?? TODO
        this.workHistory = new ArrayList<>(workHistory);
    }

    /**
     * @return String de una o varias líneas con los trabajos actuales,
     * determinado por {@code Employment.esActual}.
     */
    public String getTrabajosActuales() {
        String actual = "";
        for (Employment trabajo : workHistory) {
            if (trabajo.esActual()) {
                if (!actual.isEmpty())
                    actual = actual.concat("\n");
                actual = actual.concat(trabajo.getOneLiner());
            }
        }
        return actual;
    }

    /**
     * @return String de una línea con el último trabajo actual listado,
     * determinado por {@code Employment.esActual}.
     */
    public String getUltimoTrabajoActual() {
        String trabajos = getTrabajosActuales();
        int index = trabajos.lastIndexOf("\n");
        if (index < 0)
            return trabajos;
        return trabajos.substring(index);
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

    public String toJSON() {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        return gson.toJson(this);
    }

    public static User parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, User.class);

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
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
            Log.e("User", "No se convirtió correctamente, bizarro, ¿culpa de gson?");
            return null;
        }
    }
}
