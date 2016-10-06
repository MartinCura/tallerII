package ar.fiuba.jobify.shared_server_api;

import android.app.Activity;

/**
 * Created by martín on 06/09/16.
 * Simple estructura que contiene un skill.
 */
public class Skill implements Nombrable {

    String
            name = "",
            description = "",
            category = "";


    // copy constructor
    private Skill(Skill o) {
        this.name = o.name;
        this.description = o.description;
        this.category = o.category;
    }

    public String getName() {
        return name;
    }
    public String getNombre() {
        return getName();
    }

    public String getDescription() {
        return description;
    }

    public String getCategory() {
        return category;
    }

    // Solo permite obtener un Skill que ya exista en el SharedData
    public static Skill create(Activity activity, String skillName) {
        SkillsResponse skr = SharedDataSingleton.getInstance(activity).getSkillsResponse();
        Skill found;
        if ((found = skr.findSkill(skillName)) == null)
            throw new IllegalArgumentException();
        return new Skill(found);
    }
}
