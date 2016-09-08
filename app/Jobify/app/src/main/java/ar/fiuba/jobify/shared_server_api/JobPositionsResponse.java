package ar.fiuba.jobify.shared_server_api;

import android.util.Log;
import android.widget.Toast;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonSyntaxException;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by martín on 06/09/16.
 */
public class JobPositionsResponse {

    List<JobPosition> jobPositions;
    ResponseMetadata metadata;


    public JobPositionsResponse() {
        jobPositions = new ArrayList<>();
    }

    public static JobPositionsResponse parseJSON(String response) {
        Gson gson = new GsonBuilder()
                .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
                .create();

        try {
            JobPositionsResponse jobPositionsResponse = gson.fromJson(response, JobPositionsResponse.class);
            return jobPositionsResponse;

        } catch (JsonSyntaxException e) {
            Log.e("API", "Json Syntax exception!");
            e.printStackTrace();
            return null;
        }
    }

    public List<JobPosition> getJobPositions() {
        return jobPositions;
    }

    public ResponseMetadata getMetadata() {
        return metadata;
    }
}
