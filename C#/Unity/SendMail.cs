using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using System;
using System.Net;
using System.Net.Mail;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
using System.IO;

public class SendMail : MonoBehaviour {

    public string mailTo = "123456@gmail.com";
    private string mailFrom = "123456@gmail.com";
    public string playerFrom = "";
    public string subject = "Bug";
    public string body = "bug";
    public static List<String> LOG = new List<String>();

    public void OnEnable() {
        for(int i = 0; i < this.transform.childCount; i++) {
            Transform t = this.transform.GetChild(i);
            for(int j = 0; j < t.transform.childCount; j++) {
                Transform tr = t.GetChild(j);
                InputField inF = tr.gameObject.GetComponent<InputField>();
                if(inF != null) {
                    inF.text = "";
                }
            }
        }
    }

    public static void Logger(string condition, string stackTrace, LogType type) {
        string toAdd = "";
        toAdd += type.ToString();
        toAdd += "\n\t" + condition;
        toAdd += "\n\t\t" + stackTrace + "\n\n\n";
        LOG.Add(toAdd);
    }

    public void SetTo(string to) {
        mailTo = to;
    }

    public void SetFrom(string from) {
        playerFrom = from;
    }

    public void SetSubject(string subject) {
        this.subject = subject;
    }

    public void SetBody(string body) {
        this.body = body;
    }

    public void Send() {
        MailMessage mail = new MailMessage();

        mail.From = new MailAddress(mailFrom);
        mail.To.Add(mailTo);
        mail.Subject = subject;
        mail.Body = "Player email: " + playerFrom + "\n\n";
        mail.Body += body;
        AddPlayerLog(mail);
        SmtpClient smtpServer = new SmtpClient("smtp.gmail.com");
        smtpServer.Port = 587;
        smtpServer.Credentials = new System.Net.NetworkCredential("123456@gmail.com", "123456") as ICredentialsByHost;
        smtpServer.EnableSsl = true;
        ServicePointManager.ServerCertificateValidationCallback =
            delegate (object s, X509Certificate certificate, X509Chain chain, SslPolicyErrors sslPolicyErrors)
            { return true; };
        smtpServer.Send(mail);
        Debug.Log("success");
    }

    public void AddPlayerLog(MailMessage mail) {
        mail.Body += "\n---------------------------------------------------\n";
        for(int i = 0; i < LOG.Count; i++) {
            mail.Body += LOG[i] + "\n";
        }
    }
}
