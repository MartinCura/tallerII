package ar.fiuba.jobify;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.support.design.widget.CollapsingToolbarLayout;
import android.support.v4.app.Fragment;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.view.ViewCompat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.ParseError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.VolleyLog;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.util.Random;

import ar.fiuba.jobify.app_server_api.User;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link PerfilFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link PerfilFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class PerfilFragment extends Fragment {

    private final String LOG_TAG = PerfilFragment.class.getSimpleName();

    private static final String EXTRA_IMAGE = "ar.fiuba.jobify.extraImage";

    private TextView textView;//
    private CollapsingToolbarLayout collapsingToolbarLayout;

    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    public PerfilFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment PerfilFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static PerfilFragment newInstance(String param1, String param2) {
        PerfilFragment fragment = new PerfilFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }


//        ViewCompat.setTransitionName(
//                getActivity().findViewById(R.id.perfil_app_bar_layout), EXTRA_IMAGE);//
//
//        collapsingToolbarLayout = (CollapsingToolbarLayout) getActivity().findViewById(R.id.perfil_collapsing_toolbar);
//        collapsingToolbarLayout.setTitle(getString(R.string.perfil_nombre_y_apellido_default));
//        collapsingToolbarLayout.setExpandedTitleColor(getResources().getColor(android.R.color.transparent));

//        updateProfileInformation();//
        refreshProfileInformation();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View rootView = inflater.inflate(R.layout.fragment_perfil, container, false);

        textView = (TextView) rootView.findViewById(R.id.textview_perfil_nombre_y_apellido);

        return rootView;
    }

    // TODO: Rename method, update argument and hook method into UI event
    public void onButtonPressed(Uri uri) {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    @Override
    public void onStop() {
        super.onStop();

        if (RequestQueueSingleton.hasRequestQueue()) {

            RequestQueue mRequestQueue = RequestQueueSingleton
                    .getInstance(this.getActivity().getApplicationContext())
                    .getRequestQueue();
            mRequestQueue.cancelAll(LOG_TAG);
        }
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p/>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }


    private String getAppServerBaseURL() {
        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(getActivity());
        String ip = sharedPref.getString("pref_appServer_ip", getString(R.string.pref_default_appServer_ip));
        String puerto = sharedPref.getString("pref_appServer_puerto", getString(R.string.pref_default_appServer_puerto));

        String baseURL = "http://" + ip + ":" + puerto + "/";
        return baseURL;
    }

    public void refreshProfileInformation() {

        Random rand = new Random();
        int unId = rand.nextInt(100);// hardcodeo fuerte para CP1

        Uri builtUri = Uri.parse(getAppServerBaseURL()).buildUpon()
                .appendPath("users") //;// semi hardcodeado
                .appendPath(Integer.toString(unId))
                .build();
        String url = builtUri.toString();

        Log.d(LOG_TAG, "urrrrrrrrl: "+url);//

        JsonObjectRequest jsObjRequest = new JsonObjectRequest
                (Request.Method.GET, url, null, new Response.Listener<JSONObject>() {

                    @Override
                    public void onResponse(JSONObject response) {
                        User mUser = User.parseJSON(response.toString());

                        int idUsuario = mUser.getId();
                        String nombreUsuario = mUser.getName();

                        Toast.makeText(getActivity(), "nombre obtenido: "+nombreUsuario+
                                "\npara usuario de id "+idUsuario, Toast.LENGTH_SHORT).show();
                        textView.setText("id: "+idUsuario+"\nnombre: "+nombreUsuario);

//                        collapsingToolbarLayout.setTitle(auxString);
                    }

                }, new Response.ErrorListener() {

                    @Override
                    public void onErrorResponse(VolleyError error) {
//                        error.printStackTrace();//
                        Log.d(LOG_TAG, "errortostring "+error.toString());
//                        textView.setText("[No se obtuvo nada de la URL hardcodeada.]");
//                        Toast.makeText(getActivity(), ":(", Toast.LENGTH_SHORT).show();
                        if (error instanceof ParseError && error.getCause() instanceof JSONException) {
                            Log.d(LOG_TAG, "JSONException! Intento refreshear de nuevo...");
                            refreshProfileInformation();
                        }
                    }
                });

        jsObjRequest.setTag(LOG_TAG);

        RequestQueueSingleton.getInstance(this.getActivity().getApplicationContext())
                .addToRequestQueue(jsObjRequest);
    }

    public void updateProfileInformation() {

        String username = "Robert";//

        Uri builtUri = Uri.parse(getAppServerBaseURL()).buildUpon()
                .appendPath("user")
                .appendPath(username)
                .build();
        String url = builtUri.toString();

        final String mRequestBody = "Lottto";

        StringRequest mStringRequest = new StringRequest(Request.Method.POST,
                url, new Response.Listener<String>() {

                    @Override
                    public void onResponse(String response) {
                        Log.i("VOLLEY", response);
                    }

                }, new Response.ErrorListener() {

                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.e(LOG_TAG, "stringoferror "+error.toString());
                        textView.setText("[No se obtuvo nada de la URL hardcodeada.]");
                        Toast.makeText(getActivity(), ":(", Toast.LENGTH_SHORT).show();
                    }
                }) {

            @Override
            public byte[] getBody() throws AuthFailureError {
                try {
                    return mRequestBody == null ? null : mRequestBody.getBytes("utf-8");
                } catch (UnsupportedEncodingException uee) {
                    VolleyLog.wtf("Unsupported Encoding while trying to get the bytes of %s using %s",
                            mRequestBody, "utf-8");
                    return null;
                }
            }

//            @Override
//            protected Response<String> parseNetworkResponse(NetworkResponse response) {
//                String responseString = "";
//                if (response != null) {
//                    responseString = String.valueOf(response.statusCode);
//                    // can get more details such as response.headers
//                }
//                return Response.success(responseString, HttpHeaderParser.parseCacheHeaders(response));
//            }
        };

        mStringRequest.setTag(LOG_TAG);

        RequestQueueSingleton.getInstance(this.getActivity().getApplicationContext())
                .addToRequestQueue(mStringRequest);
    }
}