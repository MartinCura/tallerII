package ar.fiuba.jobify.shared_server_api;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by root on 06/09/16.
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

        JobPositionsResponse jobPositionResponse = gson.fromJson(response, JobPositionsResponse.class);
        return jobPositionResponse;
    }

    public List<JobPosition> getJobPositions() {
        return jobPositions;
    }

    public ResponseMetadata getMetadata() {
        return metadata;
    }
}
