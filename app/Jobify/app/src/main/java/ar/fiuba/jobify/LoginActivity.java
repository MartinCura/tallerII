package ar.fiuba.jobify;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.support.annotation.NonNull;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.app.LoaderManager.LoaderCallbacks;

import android.content.CursorLoader;
import android.content.Loader;
import android.database.Cursor;
import android.net.Uri;

import android.os.Build;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.NetworkResponse;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.HttpHeaderParser;


import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.app_server_api.LoginRequest;
import ar.fiuba.jobify.app_server_api.LoginResponse;

import static android.Manifest.permission.READ_CONTACTS;

/**
 * A login screen that offers login via email/password.
 */
public class LoginActivity extends AppCompatActivity implements LoaderCallbacks<Cursor> {

    private final static String LOG_TAG = LoginActivity.class.getSimpleName();

    /**
     * Id to identity READ_CONTACTS permission request.
     */
    private static final int REQUEST_READ_CONTACTS = 0;

    // UI references.
    private AutoCompleteTextView mEmailView;
    private EditText mPasswordView;
    private View mProgressView;
    private View mLoginFormView;

    private boolean isNewUser = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //Log.d("mylog", FirebaseInstanceId.getInstance().getToken());
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        // Set up the login form.
        mEmailView = (AutoCompleteTextView) findViewById(R.id.email);
        populateAutoComplete();

        mPasswordView = (EditText) findViewById(R.id.password);
        mPasswordView.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == R.id.login || id == EditorInfo.IME_NULL) {
                    attemptLogin();
                    return true;
                }
                return false;
            }
        });

        Button mEmailSignInButton = (Button) findViewById(R.id.email_sign_in_button);
        if (mEmailSignInButton != null) {
            mEmailSignInButton.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View view) {
                    attemptLogin();
                }
            });
        }

        Button mRegisterButton = (Button) findViewById(R.id.email_register_button);
        if (mRegisterButton != null) {
            mRegisterButton.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View view) {
                    attemptRegister();
                }
            });
        }

        mLoginFormView = findViewById(R.id.login_form);
        mProgressView = findViewById(R.id.login_progress);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.nav_drawer, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            startActivity(new Intent(this, SettingsActivity.class));
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private void populateAutoComplete() {
        if (!mayRequestContacts()) {
            return;
        }
        getLoaderManager().initLoader(0, null, this);
    }

    private boolean mayRequestContacts() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            return true;
        }
        if (checkSelfPermission(READ_CONTACTS) == PackageManager.PERMISSION_GRANTED) {
            return true;
        }
//        if (shouldShowRequestPermissionRationale(READ_CONTACTS)) {
        Toast.makeText(this, R.string.permission_rationale, Toast.LENGTH_LONG).show();
        Snackbar.make(mEmailView, R.string.permission_rationale_acceptance, Snackbar.LENGTH_INDEFINITE)
                .setAction(android.R.string.ok, new View.OnClickListener() {
                    @Override
                    @TargetApi(Build.VERSION_CODES.M)
                    public void onClick(View v) {
                        requestPermissions(new String[]{READ_CONTACTS}, REQUEST_READ_CONTACTS);
                    }
                }).show();
//        } else {
//            requestPermissions(new String[]{READ_CONTACTS}, REQUEST_READ_CONTACTS);
//        }
        return false;
    }

    /**
     * Callback received when a permissions request has been completed.
     */
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (requestCode == REQUEST_READ_CONTACTS) {
            if (grantResults.length == 1 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                populateAutoComplete();
            }
        }
    }

    /**
     * If there are form errors (invalid email, missing fields, etc.),
     * the errors are presented and false is returned.
     */
    private boolean validateLoginInput() {
        // Reset errors.
        mEmailView.setError(null);
        mPasswordView.setError(null);

        // Store values at the time of the login attempt.
        String email = mEmailView.getText().toString();
        String password = mPasswordView.getText().toString();

        boolean cancel = false;
        View focusView = null;

        // Check for a valid password, if the user entered one.
        if (TextUtils.isEmpty(password)) {
            mPasswordView.setError(getString(R.string.login_invalid_empty_password));
            focusView = mPasswordView;
            cancel = true;
        } else if (!isPasswordValid(password)) {
            mPasswordView.setError(getString(R.string.error_invalid_password));
            focusView = mPasswordView;
            cancel = true;
        }

        // Check for a valid email address.
        if (TextUtils.isEmpty(email)) {
            mEmailView.setError(getString(R.string.error_field_required));
            focusView = mEmailView;
            cancel = true;
        } else if (!isEmailValid(email)) {
            mEmailView.setError(getString(R.string.error_invalid_email));
            focusView = mEmailView;
            cancel = true;
        }
        if (cancel) {
            // There was an error; don't attempt login and focus the first
            // form field with an error.
            focusView.requestFocus();
        }
        return !cancel;
    }

    /**
     * Attempts to sign in the account specified by the login form.
     */
    private void attemptLogin() {
        // Store values at the time of the login attempt.
        String email = mEmailView.getText().toString();
        String password = mPasswordView.getText().toString();

        if (email.isEmpty() && password.isEmpty()) {
            fakeLogin();
            return;
        }

        if (validateLoginInput()) {
            // Show a progress spinner, and kick off a background task to
            // perform the user login attempt.
            showProgress(true);

            final Activity activity = this;
            final Context ctx = getApplicationContext();
            JSONObject jsonRequest = new LoginRequest(email, password).toJsonObject();

            Utils.postJsonToAppServer(this, getString(R.string.get_login_path), jsonRequest,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            showProgress(false);

                            LoginResponse loginResponse = LoginResponse.parseJSON(response.toString());
                            if (loginResponse == null) {
                                Log.e(LOG_TAG, "Error de parseo de LoginResponse");
                                return;
                            }
                            Toast.makeText(LoginActivity.this, "Login correcto", Toast.LENGTH_LONG)
                                    .show();//

                            guardarConnectedUserData(loginResponse);
                            Utils.iniciarPerfilActivity(activity, loginResponse.getId(), isNewUser);
                            finish();

                        }
                    }, new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            showProgress(false);
                            if (error.networkResponse != null) {
                                int statusCode = error.networkResponse.statusCode;
                                Log.d(LOG_TAG, "Login error status code: " + statusCode);

                                switch (statusCode) { // hardcodeado?
                                    ///
                                    case 400:
                                        try {
                                            NetworkResponse response = error.networkResponse;
                                            String res = new String(response.data,
                                                    HttpHeaderParser.parseCharset(response.headers, "utf-8"));
                                            // Now you can use any deserializer to make sense of data
                                            Log.d(LOG_TAG, "json string: "+res);
                                            new JSONObject(res);                    //;//
                                        } catch (UnsupportedEncodingException e1) {
                                            Log.d(LOG_TAG, "..couldn't decode data?");
                                            e1.printStackTrace();
                                        } catch (JSONException e2) {
                                            Log.d(LOG_TAG, "..not a json object?");
                                            e2.printStackTrace();
                                        }
                                        break;
                                    ///
                                    case 404:
                                        Toast.makeText(ctx, "Email no registrado", Toast.LENGTH_LONG)
                                                .show();
                                        break;
                                    case 401:
                                        mPasswordView.setError(getString(R.string.error_incorrect_password));
                                        mPasswordView.requestFocus();
                                        break;
                                    default:
                                        Toast.makeText(ctx, "Login error status code: "+ statusCode,
                                                Toast.LENGTH_LONG)
                                                .show();//
                                        error.printStackTrace();//
                                } // TODO: Otros status codes?
                            }
                        }
                    }, LOG_TAG);
        }
    }

    private void attemptRegister() {

        // Store values at the time of the login attempt.
        String email = mEmailView.getText().toString();
        String password = mPasswordView.getText().toString();

        if (validateLoginInput()) {
            // Show a progress spinner, and kick off a background task to
            // perform the user login attempt.
            showProgress(true);

            final Context ctx = getApplicationContext();

            JSONObject jsonRequest = new LoginRequest(email, password).toJsonObject();
            Log.d(LOG_TAG, "POST de registro:\n"+jsonRequest.toString());//

            Utils.postJsonToAppServer(this, getString(R.string.post_user_path), jsonRequest,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            showProgress(false);

                            LoginResponse loginResponse = LoginResponse.parseJSON(response.toString());
                            if (loginResponse == null) {
                                Log.e(LOG_TAG, "Error de parseo de LoginResponse");
                                return;
                            }
                            Toast.makeText(LoginActivity.this, "Registro correcto\n" +
                                    "Id: " + loginResponse.getId(), Toast.LENGTH_LONG)
                                    .show();//

                            isNewUser = true;
                            attemptLogin();
//                            guardarConnectedUserData(loginResponse);
//                            Utils.iniciarPerfilActivity(activity, loginResponse.getId(), isNewUser);
//                            finish();

                        }

                    }, new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            showProgress(false);
                            if (error.networkResponse != null) {
                                int statusCode = error.networkResponse.statusCode;
                                Log.d(LOG_TAG, "Register error status code: " + statusCode);
                                if (error.networkResponse.statusCode == 409) {// hardcodeo
                                    Toast.makeText(ctx, "Email ya registrado", Toast.LENGTH_LONG)
                                            .show();
                                } // TODO: Otros status codes?
                            }
                        }
                    }, LOG_TAG);
        }
    }

    /**
     * Guarda datos de login del LoginResponse, id y token.
     */
    private void guardarConnectedUserData(LoginResponse loginResponse) {
        SharedPreferences.Editor editor =
                getSharedPreferences(getString(R.string.shared_pref_connected_user), 0)
                        .edit();
        editor.putLong(getString(R.string.stored_connected_user_id), loginResponse.getId());
        editor.putString(getString(R.string.stored_connected_user_token), loginResponse.getToken());
        editor.apply();
    }

    private boolean isEmailValid(String email) {
        //TODO: Replace this with your own logic
        return email.contains("@");
    }

    private boolean isPasswordValid(String password) {
        //TODO: Replace this with your own logic
        return password.length() > 4;
    }

    /**
     * Shows the progress UI and hides the login form.
     */
    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private void showProgress(final boolean show) {
        // On Honeycomb MR2 we have the ViewPropertyAnimator APIs, which allow
        // for very easy animations. If available, use these APIs to fade-in
        // the progress spinner.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
            int shortAnimTime = getResources().getInteger(android.R.integer.config_shortAnimTime);

            mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
            mLoginFormView.animate().setDuration(shortAnimTime).alpha(
                    show ? 0 : 1).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
                }
            });

            mProgressView.setVisibility(show ? View.VISIBLE : View.GONE);
            mProgressView.animate().setDuration(shortAnimTime).alpha(
                    show ? 1 : 0).setListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    mProgressView.setVisibility(show ? View.VISIBLE : View.GONE);
                }
            });
        } else {
            // The ViewPropertyAnimator APIs are not available, so simply show
            // and hide the relevant UI components.
            mProgressView.setVisibility(show ? View.VISIBLE : View.GONE);
            mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
        }
    }

    @Override
    public Loader<Cursor> onCreateLoader(int i, Bundle bundle) {
        return new CursorLoader(this,
                // Retrieve data rows for the device user's 'profile' contact.
                Uri.withAppendedPath(ContactsContract.Profile.CONTENT_URI,
                        ContactsContract.Contacts.Data.CONTENT_DIRECTORY), ProfileQuery.PROJECTION,

                // Select only email addresses.
                ContactsContract.Contacts.Data.MIMETYPE +
                        " = ?", new String[]{ContactsContract.CommonDataKinds.Email
                .CONTENT_ITEM_TYPE},

                // Show primary email addresses first. Note that there won't be
                // a primary email address if the user hasn't specified one.
                ContactsContract.Contacts.Data.IS_PRIMARY + " DESC");
    }

    @Override
    public void onLoadFinished(Loader<Cursor> cursorLoader, Cursor cursor) {
        List<String> emails = new ArrayList<>();
        cursor.moveToFirst();
        while (!cursor.isAfterLast()) {
            emails.add(cursor.getString(ProfileQuery.ADDRESS));
            cursor.moveToNext();
        }

        addEmailsToAutoComplete(emails);
    }

    @Override
    public void onLoaderReset(Loader<Cursor> cursorLoader) {

    }

    private void addEmailsToAutoComplete(List<String> emailAddressCollection) {
        //Create adapter to tell the AutoCompleteTextView what to show in its dropdown list.
        ArrayAdapter<String> adapter =
                new ArrayAdapter<>(LoginActivity.this,
                        android.R.layout.simple_dropdown_item_1line, emailAddressCollection);

        mEmailView.setAdapter(adapter);
    }


    private interface ProfileQuery {
        String[] PROJECTION = {
                ContactsContract.CommonDataKinds.Email.ADDRESS,
                ContactsContract.CommonDataKinds.Email.IS_PRIMARY,
        };

        int ADDRESS = 0;
//        int IS_PRIMARY = 1;
    }

    // PARA TESTING, ONLY DEBUGGING, TODO: BORRAR en final
    private void fakeLogin() {
        long connectedUserId = 2L;
        String mail = "jane@doe.com", pass = "123abc";//"123"; // Depende del branch...
        mEmailView.setText(mail);
        mPasswordView.setText(pass);

        Toast.makeText(LoginActivity.this, "Automatic fake login\n" +
                "user id: "+connectedUserId, Toast.LENGTH_LONG)
                .show();

        attemptLogin();

//        LoginResponse loginResponse =
//                LoginResponse.parseJson("{\"id\": "+connectedUserId+", \"token\": ");
//        guardarConnectedUserData(loginResponse);
//        Utils.iniciarPerfilActivity(this, connectedUserId, isNewUser);
//        finish();
    }//;//
}
