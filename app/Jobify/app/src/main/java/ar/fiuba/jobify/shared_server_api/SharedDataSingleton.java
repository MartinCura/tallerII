package ar.fiuba.jobify.shared_server_api;

import android.app.Activity;
import android.util.Log;

import java.util.List;

import ar.fiuba.jobify.R;
import ar.fiuba.jobify.utils.Utils;

/**
 * Created by martín on 03/10/16.
 * Singleton to store existing shared data information.
 */
public class SharedDataSingleton {

    static SharedDataSingleton mInstance;
    CategoriesResponse mCategoriesResponse;
    JobPositionsResponse mJobPositionsResponse;
    SkillsResponse mSkillsResponse;
    static Activity mActivity;


    private SharedDataSingleton(Activity activity) {
        mActivity = activity;

        mCategoriesResponse = getCategoriesResponse();
        mJobPositionsResponse = getJobPositionsResponse();
        mSkillsResponse = getSkillsResponse();
    }

    public static synchronized SharedDataSingleton getInstance(Activity activity) {
        if (mInstance == null) {
            mInstance = new SharedDataSingleton(activity);
        }
        return mInstance;
    }

    public static boolean hasInstance() {
        return (mInstance != null);
    }

    public CategoriesResponse getCategoriesResponse() {
        if (mCategoriesResponse == null) {
            String json = Utils.getSharedServerDataJsonString(mActivity,
                    CategoriesResponse.class.getSimpleName(), R.string.ss_get_categories_path);
            mCategoriesResponse = CategoriesResponse.parseJSON(json);

            if (mCategoriesResponse == null) {
                Log.d("SharedData", "Se recibió una mala CategoriesResponse");
                Utils.borrarSharedServerData(mActivity, CategoriesResponse.class.getSimpleName());
                mCategoriesResponse = null;
                return null;
            }
        }
        return mCategoriesResponse;
    }

    public List<Category> getCategories() throws NoDataException {
        CategoriesResponse cr = getCategoriesResponse();
        if (cr == null)
            throw new NoDataException("categories");
        return cr.getCategories();
    }

    public JobPositionsResponse getJobPositionsResponse() {

        if (mJobPositionsResponse == null) {
            String json = Utils.getSharedServerDataJsonString(mActivity,
                    JobPositionsResponse.class.getSimpleName(), R.string.ss_get_job_positions_path);
            mJobPositionsResponse = JobPositionsResponse.parseJSON(json);

            if (mJobPositionsResponse == null) {
                Log.d("SharedData", "Se recibió una mala JobPositionsResponse");
                Utils.borrarSharedServerData(mActivity, JobPositionsResponse.class.getSimpleName());
                mJobPositionsResponse = null;
                return null;
            }
        }
        return mJobPositionsResponse;
    }

    public List<JobPosition> getJobPositions() throws NoDataException {
        JobPositionsResponse jpr = getJobPositionsResponse();
        if (jpr == null)
            throw new NoDataException("job_positions");
        return jpr.getJobPositions();
    }

    public SkillsResponse getSkillsResponse() {

        if (mSkillsResponse == null) {
            String json = Utils.getSharedServerDataJsonString(mActivity,
                    SkillsResponse.class.getSimpleName(), R.string.ss_get_skills_path);
            mSkillsResponse = SkillsResponse.parseJSON(json);

            if (mSkillsResponse == null) {
                Log.d("SharedData", "Se recibió una mala JobPositionsResponse");
                Utils.borrarSharedServerData(mActivity, SkillsResponse.class.getSimpleName());
                mSkillsResponse = null;
                return null;
            }
        }
        return mSkillsResponse;
    }

    public List<Skill> getSkills() throws NoDataException {
        SkillsResponse sr = getSkillsResponse();
        if (sr == null)
            throw new NoDataException("skills");
        return sr.getSkills();
    }


    public class NoDataException extends Exception {
        public NoDataException(String message) {
            super(message);
        }
    }
}
