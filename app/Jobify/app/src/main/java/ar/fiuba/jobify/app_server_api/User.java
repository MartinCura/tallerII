package ar.fiuba.jobify.app_server_api;

import android.support.annotation.Nullable;
import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;
import com.google.gson.annotations.SerializedName;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import ar.fiuba.jobify.utils.Utils;
import ar.fiuba.jobify.shared_server_api.Skill;

/**
 * Created by martín on 08/09/16.
 * Clase que maneja todos los elementos de un usuario del sistema.
 * Para la creación de un nuevo usuario, usar el constructor de e-mail.
 * La única otra forma permitida por el momento de crear uno entero es a partir de un Json,
 * de esa forma evitando que se toque el id, elemento decidido únicamente por el servidor.
 */
public class User {

    public final static int MAX_CHAR_NAMES = 35;
    public final static int MAX_CHAR_SUMMARY = 1000;

    @SerializedName(value="id", alternate={"with"})
    long id;
    String  firstName = "",
            lastName = "",
            email = "",
            city = "",
            dateOfBirth = "1/1/1990",
            summary = "";

    Locacion location;

    @SerializedName(value="cantidadRecomendaciones", alternate={"cantRecomendaciones", "unread_count"})
    long cantidadRecomendaciones = -1;
    long[] recommendations;

    List<Skill> skills;
    List<Employment> workHistory;


    @SuppressWarnings("unused")
    public User() {
        this.skills = new ArrayList<>();
        this.workHistory = new ArrayList<>();
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

    public User(Contact c) {
        this();
        this.id = c.getId();
        this.firstName = c.getFirstName();
        this.lastName = c.getLastName();
        Employment currJob = c.getCurrentJob();
        if (currJob != null)
            this.workHistory.add(currJob);
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
//    public Locacion getLocation() {
//        return location;
//    }
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
//    public long[] getRecomendaciones() {
//        return recommendations;
//    }

    public int getDiaNacimiento() {
        try {
            return Integer.parseInt(dateOfBirth.split("/")[0]);
        } catch (NumberFormatException ex) {
            dateOfBirth = "01/01/1990";
            return getDiaNacimiento();
        }
    }
    public int getMesNacimiento() {
        return Integer.parseInt(dateOfBirth.split("/")[1]);
    }
    public int getAnioNacimiento() {
        return Integer.parseInt(dateOfBirth.split("/")[2]);
    }

    public long getCantRecomendaciones() {
        if (cantidadRecomendaciones < 0) {
            if (recommendations == null)
                cantidadRecomendaciones = 0;
            else
                cantidadRecomendaciones = recommendations.length;
        }
        return cantidadRecomendaciones;
    }

    // Para usar por ConversationsResponse
    public long getUnreadCount() {
        if (cantidadRecomendaciones < 0)
            return 0;
        return cantidadRecomendaciones;
    }

    public boolean fueRecomendadoPor(long pepito) {
        if (recommendations == null)
            return false;
        for (long i : recommendations)
            if (i == pepito)
                return true;
        return false;
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
        // if?
        this.city = city;
        return true;
    }

    public boolean setLocacion(double latitud, double longitud) {
        if (location == null) {
            location = new Locacion();
        }
        return location.setLocation(latitud, longitud);
    }

    // No normaliza la cantidad de dígitos
    public boolean setDateOfBirth(int dia, int mes, int anio) {
        final Calendar c = Calendar.getInstance();
        // Mínima edad: 10 años, INADI quién
        if (!Utils.validarFecha(dia, mes, anio) || anio < 1900 || anio >= c.get(Calendar.YEAR) - 10)
            return false;
        this.dateOfBirth = dia + "/" + mes + "/" + anio;
        return true;
    }

    public boolean setSummary(String summary) {
        if (summary.length() > MAX_CHAR_SUMMARY)
            return false;
        this.summary = summary;
        return true;
    }

    // Reemplaza los Skill ya guardados
    public void setSkills(List<Skill> skills) {
        // Chequeos?
        this.skills = new ArrayList<>(skills);
    }

    // Reemplaza los Employment ya guardados
    public void setWorkHistory(List<Employment> workHistory) {
        // Chequeos?
        this.workHistory = new ArrayList<>(workHistory);
    }

    /**
     * @return String de una o varias líneas con los trabajos actuales,
     * determinado por {@code Employment.esActual}.
     */
    public String getTrabajosActuales() {
        String actual = "";
        if (workHistory == null)
            return actual;
        for (Employment trabajo : workHistory) {
            if (trabajo != null && trabajo.esActual()) {
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

    public List<String> getListaJobs() {
        List<String> lista = new ArrayList<>();
        if (getWorkHistory() != null) {
            for (Employment job : getWorkHistory()) {
                if (job != null)
                    lista.add(job.getCompleto());
            }
        }
        return lista;
    }

    public List<String> getListaSkills() {
        List<String> lista = new ArrayList<>();
        if (getSkills() != null) {
            for (Skill skill : getSkills()) {
                if (skill != null)
                    lista.add(skill.getName());
            }
        }
        return lista;
    }

    private void addEmployment(Employment emp) {
        if (emp == null)
            return;
        if (this.workHistory == null)
            this.workHistory = new ArrayList<>();
        this.workHistory.add(emp);
    }


    @Nullable
    public static User parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            User user = gson.fromJson(response, User.class);
            // Para user reducido, que puede tener un campo "last_job" o "current_job"
            try {
                user.addEmployment(gson.fromJson(
                        (new JSONObject(response)).getJSONObject("current_job").toString(), // hardcodeo
                        Employment.class)
                );
            } catch (JSONException ex) {/**/}
            try {
                user.addEmployment(gson.fromJson(
                        (new JSONObject(response)).getJSONObject("last_job").toString(), // hardcodeo
                        Employment.class)
                );
            } catch (JSONException ex) {/**/}

            return user;

        } catch (JsonSyntaxException ex) {
            Log.e("API", "Json Syntax exception!");
            ex.printStackTrace();
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
