package uvigo.teleco.deny.seguridad15;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

/**Esta clase va a recibir una señal cuando el movil se enciende
 * Created by Deny on 16/11/2014.
 */
public class BOOTReceiver extends BroadcastReceiver {
    static final String ACTION="android.intent.action.BOOT_COMPLETED";

    @Override
    public void onReceive(Context context, Intent intent) {
        Intent i = new Intent(context, MainActivity.class); //hago un nuevo intent
        i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(i); //empiezo la main activity es decir la app
    }
}
