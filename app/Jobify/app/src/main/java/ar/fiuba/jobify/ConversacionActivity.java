package ar.fiuba.jobify;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.LayoutRes;
import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.AbsListView;
import android.widget.BaseAdapter;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;

import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;

import ar.fiuba.jobify.app_server_api.Message;
import ar.fiuba.jobify.app_server_api.MessagesResponse;
import ar.fiuba.jobify.app_server_api.User;

public class ConversacionActivity extends NavDrawerActivity {

    private final String LOG_TAG = ConversacionActivity.class.getSimpleName();

    public final static String CORRESPONSAL_ID_MESSAGE = "ar.fiuba.jobify.CORRESPONSAL_ID_MESSAGE";

    private final int CANT_MENSAJES_POR_PAGE = 10;

    private long corresponsalID = 0;
    private String nombreCorresponsal;

    // Variable setteada con el totalCount del Metadata para saber cuándo se acabaron los mensajes;
    // si envío o recibo nuevos mensajes sin salir de la actividad, debería incrementársela.
    private long maxMensaje = 0;

    private ListView mListView;
    private MessageArrayAdapter mMessageArrayAdapter;
    private EndlessScrollListener mEndlessScrollListener;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_conversacion_drawer);

        ActionBar sab = getSupportActionBar();
        if (sab != null) sab.setDisplayHomeAsUpEnabled(true);

        // Obtengo el id del usuario cuya conversación debo mostrar
        Intent intent = getIntent();
        if (intent != null && intent.hasExtra(CORRESPONSAL_ID_MESSAGE)) {
            corresponsalID = intent.getLongExtra(CORRESPONSAL_ID_MESSAGE, corresponsalID);
        } else {
            Log.e(LOG_TAG, "No encontré corresponsal ID para hacer esto bien. Quemo todo.");
            finish();
        }

        EditText et_input = (EditText) findViewById(R.id.conversacion_entrada);

        if (et_input != null) {
           // TODO: Revisar que no arruine el Enter en el teclado del celular
           et_input.setOnEditorActionListener(new TextView.OnEditorActionListener() {
               @Override
               public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                   if (id == R.id.conversacion_enviar || id == EditorInfo.IME_NULL) {
                       enviarMensaje();
                       return true;
                   }
                   return false;
               }
           });
        }

        mListView = (ListView) findViewById(R.id.conversacion_list);
        if (mListView != null) {
            mMessageArrayAdapter = new MessageArrayAdapter(this, new ArrayList<Message>());
            mEndlessScrollListener = new EndlessScrollListener();
            mListView.setAdapter(mMessageArrayAdapter);
            mListView.setOnScrollListener(mEndlessScrollListener);
            mListView.setDivider(null);
            mListView.setDividerHeight(18);//hardcode

        } else {
            Log.e(LOG_TAG, "No se encontró la listview de mensajes! :~| Salgo ofendido.");
            finish();
        }

        // TODO: Poner un loading() ?
    }

    @Override
    public void setContentView(@LayoutRes int layoutResID) {
        super.setContentView(layoutResID);
        onCreateDrawer(R.id.conversacion_toolbar, R.id.conversacion_drawer_layout, R.id.conversacion_nav_view);
    }

    @Override
    public void onResume() {
        super.onResume();

        if (corresponsalID > 0) {
            cargarHeaderConversacion();
            cargarUltimosMensajes();
        }
    }

    public void onStop() {
        super.onStop();
        if (RequestQueueSingleton.hasRequestQueue()) {  // TODO: Llamar a esto acá? Revisar.
            RequestQueue mRequestQueue = RequestQueueSingleton
                    .getInstance(this.getApplicationContext())
                    .getRequestQueue();
            mRequestQueue.cancelAll(LOG_TAG);
        }
    }


    /**
     * Punto de entrada para notificar por nuevos mensajes
     * Por ahora la forma no es la mejor, (TODO) se podría emprolijar, leer commit message ~ mc
     */
    public void recibirMensajesNuevos() {
        // Chequear que los nuevos mensajes sean para esta conversación
        recargarMensajes();
    }


    private void cargarHeaderConversacion() {
        final TextView tv_nombre = (TextView) findViewById(R.id.conversacion_nombre);
        if (tv_nombre != null) {
            Utils.getJsonFromAppServer(this, getString(R.string.get_user_path), corresponsalID,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            User mUser = User.parseJSON(response.toString());
                            if (mUser != null) {
                                //corresponsalUser = mUser;// TODO: Guardo ese user para algo?
                                nombreCorresponsal = mUser.getFullName();
                                tv_nombre.setText(nombreCorresponsal);
                            } else {
                                Log.e(LOG_TAG, "Error de parseo de usuario, no completo header");
                            }
                        }
                    }, LOG_TAG);
        } else {
            Log.e(LOG_TAG, "No encontré el textview de nombre");
        }

        ImageButton ib_thumbnail = (ImageButton) findViewById(R.id.conversacion_thumbnail);
        String urlGetThumbnail = Utils.getAppServerUrl(this, corresponsalID,
                getString(R.string.get_thumbnail_path));
        Utils.cargarImagenDeURLenImageView(this, ib_thumbnail, urlGetThumbnail, LOG_TAG, true);
    }

    public void irAPerfil(View v) {
        Utils.iniciarPerfilActivity(this, corresponsalID, false);
    }

    private void cargarUltimosMensajes() {
        final Context ctx = this;
        HashMap<String, String> map = new HashMap<>();
        map.put("user", Long.toString(corresponsalID));
        map.put("from", "1");   // Carga desde el último mensaje
        map.put("to", Integer.toString(CANT_MENSAJES_POR_PAGE));
        String urlMensajes = Utils.getAppServerUrl(this, getString(R.string.get_messages_path),
                connectedUserID, map);

        Utils.fetchJsonFromUrl(this, Request.Method.GET, urlMensajes, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        MessagesResponse messagesResponse =
                                MessagesResponse.parseJSON(response.toString());

                        if (messagesResponse != null) {
                            maxMensaje = messagesResponse.getMetadata().getTotalCount();

                            /////////////// TODO DE PRUEBA, AGARRANDO DE AMBAS FORMAS DE URL ///////
//                            final ArrayList<Message> msjs = new ArrayList<>(messagesResponse.getMessages());
//                            HashMap<String, String> map = new HashMap<>();
//                            map.put("user", Long.toString(connectedUserID));
//                            map.put("from", "1");   // Carga desde el último mensaje
//                            map.put("to", Integer.toString(CANT_MENSAJES_POR_PAGE));
//                            String urlMensajes = Utils.getAppServerUrl(ctx, "messages",
//                                    corresponsalID, map);
//                            Utils.fetchJsonFromUrl(ctx, Request.Method.GET, urlMensajes, null,
//                                    new Response.Listener<JSONObject>() {
//                                        @Override
//                                        public void onResponse(JSONObject response) {
//                                            MessagesResponse messagesResponse2 =
//                                                    MessagesResponse.parseJSON(response.toString());
//
//                                            if (messagesResponse2 != null) {
//                                                msjs.addAll(messagesResponse2.getMessages());
//                                                Collections.sort(msjs, new Comparator<Message>() {
//                                                    @Override
//                                                    public int compare(Message m1, Message m2) {
//                                                        return m1.getTimestamp().compareTo(m2.getTimestamp());
//                                                    }
//                                                });
//
//                                                mMessageArrayAdapter = new MessageArrayAdapter(ctx, msjs);
//                                                mListView.setAdapter(mMessageArrayAdapter);
//
//                                            } else {
//                                                Log.e(LOG_TAG, "Error de parseo de MessagesResponse");
//                                            }
//                                        }
//                                    }, LOG_TAG);
                            /////////////// TODO ///////////////////////////////////////////////////

                            // Original //
                            mMessageArrayAdapter = new MessageArrayAdapter(ctx, messagesResponse.getMessages());
                            mListView.setAdapter(mMessageArrayAdapter);

                        } else {
                            Log.e(LOG_TAG, "Error de parseo de MessagesResponse");
                        }
                    }
                }, LOG_TAG);
    }

    private boolean cargarMasMensajes(long page) {
        return cargarMasMensajes(page, false);
    }

    /**
     * @param page es el número de páginas de resultados, comenzando en 0 para el cargado inicial
     * @param forzarCarga ignora si el límite guardado localmente ya fue alcanzado, cargando más
     */
    public boolean cargarMasMensajes(long page, boolean forzarCarga) {
        if (!forzarCarga && (page * CANT_MENSAJES_POR_PAGE >= maxMensaje)) {
            mEndlessScrollListener.desactivar();
            return false;
        }

        HashMap<String, String> map = new HashMap<>();
        map.put("user", Long.toString(corresponsalID));
        map.put("from", Long.toString(page * CANT_MENSAJES_POR_PAGE + 1));
        map.put("to",   Long.toString((page + 1) * CANT_MENSAJES_POR_PAGE));
        String urlMensajes = Utils.getAppServerUrl(this, getString(R.string.get_messages_path),
                connectedUserID, map);

        Utils.fetchJsonFromUrl(this, Request.Method.GET, urlMensajes, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        MessagesResponse messagesResponse =
                                MessagesResponse.parseJSON(response.toString());

                        if (messagesResponse != null) {
                            maxMensaje = messagesResponse.getMetadata().getTotalCount();
                            mMessageArrayAdapter.appendearMensajes(messagesResponse.getMessages());
                            mEndlessScrollListener.activar();

                        } else {
                            Log.e(LOG_TAG, "Error de parseo de MessagesResponse");
                        }
                    }
                }, LOG_TAG);
        return true;
    }

    /**
     * Limpia los mensajes y recarga
     */
    private void recargarMensajes() {
        mMessageArrayAdapter.vaciar();
        mListView.setAdapter(null);

        mListView.setAdapter(mMessageArrayAdapter = new MessageArrayAdapter(this, new ArrayList<Message>()));
        mListView.setOnScrollListener(mEndlessScrollListener = new EndlessScrollListener());

        cargarUltimosMensajes();
    }

    public void enviarMensaje(View v) {
        enviarMensaje();
    }

    private void enviarMensaje() {
        final EditText et_entrada = (EditText) findViewById(R.id.conversacion_entrada);
        if (et_entrada == null) {
            Log.e(LOG_TAG, "No encontré EditView de input");
            return;
        }

        final Context context = this;//

        final String mensajeAEnviar = et_entrada.getText().toString();
        if (!mensajeAEnviar.isEmpty()) {
            Message message = new Message(connectedUserID, corresponsalID, mensajeAEnviar);
            String urlEnvioMensaje = Utils.getAppServerUrl(this, getString(R.string.put_messages_path));

            Utils.fetchJsonFromUrl(this, Request.Method.PUT, urlEnvioMensaje, message.toJsonObject(),
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
//                            Toast.makeText(context, "Mensaje enviado", Toast.LENGTH_LONG)
//                                    .show();//
                            Log.i(LOG_TAG, "Mensaje enviado: " + mensajeAEnviar);//

                            // Vacío el input.
                            et_entrada.setText(null);

                            // Recargo los mensajes para que se muestren correctamente
                            // TODO: Otra manera más prolija? Usando recibirNuevosMensajes() ?
                            recargarMensajes();
                        }
                    }, new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            Toast.makeText(context, "¡Mensaje no enviado!", Toast.LENGTH_LONG)
                                    .show();//
                            Log.e(LOG_TAG, "Mensaje NO enviado: " + mensajeAEnviar);//
                            if (error.networkResponse != null)
                                Log.e(LOG_TAG, "Status code de mensaje no enviado: "
                                        + error.networkResponse.statusCode);
                            error.printStackTrace();
                        }
                    }, LOG_TAG);
        }
    }


    private class MessageArrayAdapter extends BaseAdapter {

        Context ctx;
        ArrayList<Message> chatMessages;

        /** Debe recibirlo ordenado por timestamp de MAYOR A MENOR */
        public MessageArrayAdapter(Context context, List<Message> messageList) {
            this.ctx = context;
            this.chatMessages = new ArrayList<>(messageList);

            ///TEST, TODO: BORRAR
//            chatMessages.add(new Message(1, 2, "hola"));
//            chatMessages.add(new Message(1, 2, "hola"));
//            chatMessages.add(new Message(1, 2, "hola"));
//            chatMessages.add(new Message(1, 2, "hola"));
//            chatMessages.add(new Message(1, 2, "hola"));
//            chatMessages.add(new Message(1, 2, "hola"));
//            chatMessages.add(new Message(1, 2, "hooooolaaaaa"));
//            chatMessages.add(new Message(1, 2, "heyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"));
//            chatMessages.add(new Message(2, 1, "holissssssssssssssssssssssssssssssssssssssss"));
//            chatMessages.add(new Message(1, 2, "holis! :D"));
//            chatMessages.add(new Message(2, 1, "holis!"));
//            chatMessages.add(new Message(2, 1, "chau :)"));
//            chatMessages.add(new Message(1, 2, "noo :("));
//            chatMessages.add(new Message(2, 1, "has disconnected"));
            ///
        }

        /** Precondición: son anteriores a los ya listados
         * y se encuentran listados de MAYOR a MENOR */
        public void appendearMensajes(List<Message> messageList) {
            this.chatMessages.addAll(messageList);
            notifyDataSetChanged();
        }

        public void vaciar() {
            this.chatMessages.clear();
            //notifyDataSetChanged();
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            ViewHolderIncoming holderIn;
            ViewHolderOutgoing holderOut;

            Message message = getItem(position);
            if (message != null) {
                switch (message.getAutoria(connectedUserID)) {
                    case PROPIO:
                        if (convertView == null) {
                            convertView = LayoutInflater.from(ctx)
                                    .inflate(R.layout.mensaje_outgoing_item, parent, false);
                            holderOut = new ViewHolderOutgoing();

                            holderOut.tv_message = (TextView) convertView
                                    .findViewById(R.id.mensaje_outgoing_message_text);
                            holderOut.tv_time = (TextView) convertView
                                    .findViewById(R.id.mensaje_outgoing_time_text);
                            holderOut.iv_status = (ImageView) convertView
                                    .findViewById(R.id.mensaje_outgoing_status);

                            convertView.setTag(holderOut);
                        } else {
                            holderOut = (ViewHolderOutgoing) convertView.getTag();
                        }

//                        holderOut.iv_status TODO?
//                        if (message.getMessageStatus() == Message.Status.DELIVERED) {
//                            holderOut.messageStatus.setImageDrawable(context.getResources().getDrawable(R.drawable.ic_double_tick));
//                        } else if (message.getMessageStatus() == Message.Status.SENT) {
//                            holderOut.messageStatus.setImageDrawable(context.getResources().getDrawable(R.drawable.ic_single_tick));
//                        }
                        holderOut.tv_message.setText(message.getMessage());
                        holderOut.tv_time.setText(message.getHoraSinSegundos());//.getTwoLiner());
                        break;

                    default:
                    case AJENO:
                        if (convertView == null) {
                            convertView = LayoutInflater.from(ctx)
                                    .inflate(R.layout.mensaje_incoming_item, parent, false);
                            holderIn = new ViewHolderIncoming();

                            holderIn.tv_author = (TextView) convertView
                                    .findViewById(R.id.mensaje_incoming_author);
                            holderIn.tv_message = (TextView) convertView
                                    .findViewById(R.id.mensaje_incoming_message_text);
                            holderIn.tv_time = (TextView) convertView
                                    .findViewById(R.id.mensaje_incoming_time_text);

                            convertView.setTag(holderIn);
                        } else {
                            holderIn = (ViewHolderIncoming) convertView.getTag();
                        }

                        holderIn.tv_author.setText(nombreCorresponsal);
//                        holderIn.tv_author.setText(message.getFrom()); TODO
                        holderIn.tv_message.setText(message.getMessage());
                        holderIn.tv_time.setText(message.getHoraSinSegundos());//.getTwoLiner());
                }
            }
            return convertView;
        }

        @Override
        public int getCount() {
            return chatMessages.size();
        }

        @Override
        public Message getItem(int position) {
            return chatMessages.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public int getViewTypeCount() {
            return Message.Autoria.values().length;
        }

        @Override
        public int getItemViewType(int position) {
            Message message = chatMessages.get(position);
            return message.getAutoria(connectedUserID).ordinal();
        }

        private class ViewHolderIncoming {
            public TextView  tv_author,
                             tv_message,
                             tv_time;
        }

        private class ViewHolderOutgoing {
            public ImageView iv_status;
            public TextView  tv_message,
                             tv_time;
        }
    }

    public class EndlessScrollListener implements AbsListView.OnScrollListener {

        private int visibleThreshold = 3;
        private int currentPage = 0;
        private int previousTotal = 0;
        private boolean loading = true;
        private boolean activado = true;

        public EndlessScrollListener() {}

//        public EndlessScrollListener(int visibleThreshold) {
//            this.visibleThreshold = visibleThreshold;
//        }

        public void activar() {
            this.activado = true;
            this.loading = true;
        }
        public void desactivar() {
            this.activado = false;
            this.loading = false;
        }

        @Override
        public void onScroll(AbsListView view, int firstVisibleItem,
                             int visibleItemCount, int totalItemCount) {
            if (!activado) return;

            if (loading) {
                if (totalItemCount > previousTotal) {
                    loading = false;
                    previousTotal = totalItemCount;
                    currentPage++;
                }
            }
            if (!loading && (totalItemCount - visibleItemCount)
                                <= (firstVisibleItem + visibleThreshold)) {
                Log.d(LOG_TAG, "duuuude");//
                if (cargarMasMensajes(currentPage))
                    loading = true;
            }
        }

        @Override
        public void onScrollStateChanged(AbsListView view, int scrollState) {
        }
    }
}
