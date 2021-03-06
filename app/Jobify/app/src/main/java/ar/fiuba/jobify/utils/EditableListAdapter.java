package ar.fiuba.jobify.utils;

import android.content.Context;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

import ar.fiuba.jobify.R;
import ar.fiuba.jobify.shared_server_api.Nombrable;

/**
 * Created by martín on 27/09/16.
 * Extensión de ArrayAdapter para crear listas donde se puede eliminar elementos,
 * utilizando el layout {@code list_item_borrable}.
 */
public class EditableListAdapter<T extends Nombrable> extends ArrayAdapter<T> {

    private final String LOG_TAG = EditableListAdapter.class.getSimpleName();

    private static final int ALTURA_MAX_SKILLS = 200; // not my best (hardcode)
    private boolean limitarAltura = false;

    private ListView adaptedListView;


    public EditableListAdapter(Context context, ListView lv, List<T> list) {
        this(context, lv, list, false);
    }

    public EditableListAdapter(Context context, ListView lv, List<T> list, boolean limitHeight) {
        super(context, R.layout.list_item_borrable, list);
        this.adaptedListView = lv;
        this.limitarAltura = limitHeight;
    }

    @Override
    public void notifyDataSetChanged() {
        super.notifyDataSetChanged();
        actualizarAlturaDeListViewHardcode(adaptedListView, this);
    }

    public boolean add(T object, boolean rechazarRepetidos) {
        if (!rechazarRepetidos || this.getPosition(object) == -1) {
            add(object);
            return true;
        }
        return false;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        View itemView = convertView;
        if (itemView == null) {
            itemView = LayoutInflater.from(getContext())
                    .inflate(R.layout.list_item_borrable, parent, false);
        }

        final T item = getItem(position);
        String itemString = item.getNombre();
        if (itemString != null) {

            TextView item_tv = (TextView) itemView.findViewById(R.id.text_list_item_editable);
            if (item_tv != null)
                item_tv.setText(itemString);

            ImageButton botonRemove = (ImageButton) itemView.findViewById(R.id.boton_borrar_item);
            if (botonRemove != null) {
                botonRemove.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        remove(item);
                        notifyDataSetChanged();
                    }
                });
            }
        }

        return itemView;
    }

    public List<T> getList() {
        ArrayList<T> list = new ArrayList<>();
        for (int i = 0; i < getCount(); i++)
            list.add(getItem(i));
        return list;
    }

    public List<String> getStringList() {
        ArrayList<String> list = new ArrayList<>();
        for (int i = 0; i < getCount(); i++)
            list.add(getItem(i).getNombre());
        return list;
    }


    public static <T extends Nombrable> EditableListAdapter<T> populateEditableList(Context context,
                                                                 ListView mListView, List<T> list) {
        return populateEditableList(context, mListView, list, false);
    }

    @Nullable
    public static <T extends Nombrable> EditableListAdapter<T> populateEditableList(Context context,
                                            ListView mListView, List<T> list, boolean limitHeight) {
        if (mListView != null) {

            EditableListAdapter<T> mAdapter =
                    new EditableListAdapter<>(context, mListView, list, limitHeight);
            mListView.setAdapter(mAdapter);
            actualizarAlturaDeListViewHardcode(mListView, mAdapter, limitHeight);

            return mAdapter;

        } else {
            Log.e("EditableListAdapter", "No se encontró el listview!");
            return null;
        }
    }

    // Asegurar que la altura de la lista modificada sea correcta.
    public static void actualizarAlturaDeListView(ListView mListView, ArrayAdapter mAdapter) {
        int totalHeight = 0;
        for (int i = 0; i < mAdapter.getCount(); i++) {
            View listItem = mAdapter.getView(i, null, mListView);
            listItem.measure(0, 0);
            totalHeight += listItem.getMeasuredHeight();
        }
        ViewGroup.LayoutParams params = mListView.getLayoutParams();
        params.height = totalHeight +
                (mListView.getDividerHeight() * (mAdapter.getCount() - 1));
        mListView.setLayoutParams(params);
    }

    public void actualizarAlturaDeListViewHardcode(ListView mListView, ArrayAdapter mAdapter) {
        actualizarAlturaDeListViewHardcode(mListView, mAdapter, this.limitarAltura);
    }

    public static void actualizarAlturaDeListViewHardcode(ListView mListView, ArrayAdapter mAdapter,
                                                          boolean limitarAltura) {
        //        int totalHeight = 0;
//        int lastHeight = 0;
        for (int i = 0; i < mAdapter.getCount(); i++) {
            View listItem = mAdapter.getView(i, null, mListView);
            listItem.measure(0, 0);
            // tramposo
//            lastHeight = listItem.getMeasuredHeight();
//            Log.d("shuddup", lastHeight+" (lastHeight");
//            totalHeight += listItem.getMeasuredHeight();
        }
        int totalHeight = 150 * mAdapter.getCount(); //hardcodeo
//        Log.d("shudduuup", "lastHeight="+lastHeight+", total="+totalHeight);

        ViewGroup.LayoutParams params = mListView.getLayoutParams();
        params.height = totalHeight +
                (mListView.getDividerHeight() * (mAdapter.getCount() - 1));

        if ((limitarAltura) && params.height > ALTURA_MAX_SKILLS) {
            params.height = ALTURA_MAX_SKILLS;
        }
        mListView.setLayoutParams(params);
    }

}
