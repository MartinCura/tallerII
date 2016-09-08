package ar.fiuba.jobify;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.util.LruCache;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.ImageLoader;
import com.android.volley.toolbox.Volley;

/**
 * Created by martín on 06/09/16.
 */


///** Interfaz de cliente */
//
//public class InterfazCliente<T extends Parseable<T>> {
//
//    private final String LOG_TAG = InterfazCliente.class.getSimpleName();
//
//    public enum RequestType {
//        GET, POST, PUT, DELETE;
//    }
//
//    private Activity activity;
//    private Class<T> classType;
//
//    public InterfazCliente(Activity activity, Class<T> classType) {
//        this.activity = activity;
//        this.classType = classType;
//    }
//
//
//    public String request(final T tInstance, final Request.Method requestType, String... pathParams) {
//        OkHttpClient client = new OkHttpClient();
//
//        HttpUrl.Builder urlBuilder = HttpUrl.parse("http://192.168.0.116:5000").newBuilder();
//        urlBuilder.addPathSegment("user");
//        urlBuilder.addPathSegment("mrrobot");
//        String url = urlBuilder.build().toString();
//
//        Log.d("asdpokds", "url built post: "+url);//
//
//        RequestBody body = RequestBody.create(null, "look above");
//        Request request = new Request.Builder()
//                .header("Authorization", "token abcd")
//                .url(url)
//                .post(body)
//                .build();
//
//        client.newCall(request).enqueue(new Callback() {
//            @Override
//            public void onFailure(Call call, IOException e) {
//                e.printStackTrace();
//            }
//
//            @Override
//            public void onResponse(Call call, final Response response) throws IOException {
//                if (!response.isSuccessful()) {
//                    throw new IOException("Unexpected code " + response);
//                }
//
//                Headers responseHeaders = response.headers();
//                for (int i = 0; i < responseHeaders.size(); i++) {
//                    Log.d("DEBUG", responseHeaders.name(i) + ": " + responseHeaders.value(i));
//                }
//                //String header = response.header("Date");
//
//                // Read data on the worker thread
//                //final String responseData = response.body().string();
//
//                //final Gson gson = new Gson();   // Con GsonBuilder puedo configurarlo.
//                //T user = gson.fromJson(response.body().charStream(), classType);
//
//                activity.runOnUiThread(new Runnable() {
//                    @Override
//                    public void run() {
//                        //Handle UI here
//                        //findViewById(R.id.loading).setVisibility(View.GONE);
//                    }
//                });
//                ///return tInstance.parseJSON(response.body().string());  // .charStream()?
//            }
//        });
//
//        return null;//
//    }
//
//}
