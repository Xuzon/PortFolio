package uvigo.teleco.deny.seguridad15;
import android.app.Activity;
import android.content.Context;
import android.telephony.TelephonyManager;
public class PhoneInfo { //tengo cada funcion pareja Activity Context por si tengo que llamarla desde una activity o un context
    public static String getPhoneNumber(Activity ac) { //una activity es un proceso en primer plano o sea que la app está activa sin embargo un context es un proceso en segundo plano
        try {
            TelephonyManager TelMan;
            TelMan= (TelephonyManager) ac.getSystemService(Context.TELEPHONY_SERVICE);
            return TelMan.getLine1Number();
        }catch (Exception e){
            return "DefaultNumber";
        }
    }
    public static String getPhoneNumber(Context c) {
        try {
            TelephonyManager TelMan;
            TelMan= (TelephonyManager) c.getSystemService(Context.TELEPHONY_SERVICE);
            return TelMan.getLine1Number();
        }catch (Exception e){
            return "DefaultNumber";
        }
    }
    public static String getMyIMEI(Activity a){
        try {
            TelephonyManager mTelephonyMgr;
            mTelephonyMgr = (TelephonyManager) a.getSystemService(Context.TELEPHONY_SERVICE);
            return mTelephonyMgr.getDeviceId();
        }
        catch (Exception e) {
            return "DefaultIMEI";
        }
    }
    public static String getMyIMEI(Context c){
        try {
            TelephonyManager mTelephonyMgr;
            mTelephonyMgr = (TelephonyManager) c.getSystemService(Context.TELEPHONY_SERVICE);
            return mTelephonyMgr.getDeviceId();
        }
        catch (Exception e) {
            return "DefaultIMEI";
        }
    }
    public static String getMyNetworkOperator(Activity a){
        try {
            TelephonyManager mTelephonyMgr;
            mTelephonyMgr = (TelephonyManager) a.getSystemService(Context.TELEPHONY_SERVICE);
            return mTelephonyMgr.getNetworkOperatorName();
        }
        catch (Exception e) {
            return "DefaultNetworkOperator";
        }
    }

    public static String getMySIMSerial(Activity a){
        try {
            TelephonyManager mTelephonyMgr;
            mTelephonyMgr = (TelephonyManager) a.getSystemService(Context.TELEPHONY_SERVICE);
            return mTelephonyMgr.getSimSerialNumber();
        }
        catch (Exception e) {
            return "DefaultSimSerial";
        }
    }
    public static String getMyIMSI(Activity a){
        try {
            TelephonyManager mTelephonyMgr;
            mTelephonyMgr = (TelephonyManager) a.getSystemService(Context.TELEPHONY_SERVICE);
            return mTelephonyMgr.getSubscriberId();
        }
        catch (Exception e) {
            return "DefaultIMSI";
        }
    }
    public static String getMyIMSI(Context c){
        try {
            TelephonyManager mTelephonyMgr;
            mTelephonyMgr = (TelephonyManager) c.getSystemService(Context.TELEPHONY_SERVICE);
            return mTelephonyMgr.getSubscriberId();
        }
        catch (Exception e) {
            return "DefaultIMSI";
        }
    }
    public static String getMyVoiceMailNumberI(Activity a){
        try {
            TelephonyManager mTelephonyMgr;
            mTelephonyMgr = (TelephonyManager) a.getSystemService(Context.TELEPHONY_SERVICE);
            return mTelephonyMgr.getVoiceMailNumber();
        }
        catch (Exception e) {
            return "DefaultVoiceMailNumber";
        }
    }
}
