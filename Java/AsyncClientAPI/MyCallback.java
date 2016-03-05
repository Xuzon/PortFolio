package noticia;

import org.apache.axis2.client.async.AxisCallback;
import org.apache.axis2.context.MessageContext;

public class MyCallback implements AxisCallback{
	public boolean espera = true;
	public String elemento;
	@Override
	public void onComplete() {
		espera = false;
	}

	@Override
	public void onError(Exception arg0) {
		elemento = arg0.toString();
	}

	@Override
	public void onFault(MessageContext arg0) {
		org.apache.axiom.soap.SOAPEnvelope envl = arg0.getEnvelope();
		org.apache.axiom.soap.SOAPBody msg = envl.getBody();
		elemento = msg.getFirstElement().getText();
	}

	@Override
	public void onMessage(MessageContext arg0) {
		org.apache.axiom.soap.SOAPEnvelope envl = arg0.getEnvelope();
		org.apache.axiom.soap.SOAPBody msg = envl.getBody();
		elemento = msg.getFirstElement().getFirstElement().getText();
	}

}
