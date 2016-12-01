package ar.fiuba.jobify.shared_server_api;

import android.util.Log;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by martín on 06/09/16.
 * Estructura API para los skills.
 */
public class SkillsResponse {

    List<Skill> skills;
    ResponseMetadata metadata;


    public SkillsResponse() {
        skills = new ArrayList<>();
    }

    public static SkillsResponse parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            return gson.fromJson(response, SkillsResponse.class);

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }

    public List<Skill> getSkills() {
        return skills;
    }

    public ResponseMetadata getMetadata() {
        return metadata;
    }

    public Skill findSkill(String skillName) {
        for (Skill sk : skills) {
            if (sk.getName().equalsIgnoreCase(skillName))
                return sk;
        }
        return null;
    }
}
