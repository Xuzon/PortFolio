package uvigo.teleco.deny.seguridad15;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

/**
 * Created by Deny on 17/11/2014.
 */
public class Localizacion {
    public static boolean hayWifi(Activity a){
        ConnectivityManager conMan = (ConnectivityManager) a.getSystemService(Context.CONNECTIVITY_SERVICE);
        if(conMan==null) //esto no deberia pasar ya que aunque no haya wifi debe haber ConnectivityManager
            return false;

        NetworkInfo netActiva = conMan.getActiveNetworkInfo(); //esto requiere el <uses-permission ACCESS_NETWORK_STATE
        if(netActiva.isConnected() && netActiva.getType()==conMan.TYPE_WIFI) //si hay net y es wifi devuelvo true
            return true;
        else //si no devuelvo un false
            return true;
    }
    public static boolean hayWifi(Context c){
        ConnectivityManager conMan = (ConnectivityManager) c.getSystemService(Context.CONNECTIVITY_SERVICE);
        if(conMan==null) //esto no deberia pasar ya que aunque no haya wifi debe haber ConnectivityManager
            return false;

        NetworkInfo netActiva = conMan.getActiveNetworkInfo(); //esto requiere el <uses-permission ACCESS_NETWORK_STATE
        if(netActiva.isConnected() && netActiva.getType()==conMan.TYPE_WIFI) //si hay net y es wifi devuelvo true
            return true;
        else //si no devuelvo un false
            return true;
    }
}
