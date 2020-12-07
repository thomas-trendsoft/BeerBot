package org.krieger.beerbot.client;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import org.krieger.beerbot.data.Map;
import org.krieger.beerbot.data.Position;

/**
 * Basic beer bot client 
 * 
 * @author tkrieger
 *
 */
public class BeerNetClient {
	
	/**
	 * client socket
	 */
	private Socket socket;
	
	/**
	 * hostname from beerbot
	 */
	private String hostname;
	
	/**
	 * output stream
	 */
	private OutputStream output;
	
	/**
	 * input stream
	 */
	private InputStream input;
	
	/**
	 * default constructor 
	 * 
	 * @param hostname
	 */
	public BeerNetClient(String hostname) {
		this.hostname = hostname;
	}
	
	/**
	 * connect to beerbot server
	 * 
	 * @throws UnknownHostException
	 * @throws IOException
	 */
	public void connect() throws UnknownHostException, IOException {
		this.socket   = new Socket(hostname, 10101);
		output = this.socket.getOutputStream();
	}

	public Position pullPosition() {
		return null;
	}
	
	public Map pullMap() {
		return null;
	}
	
	public String pullStatus() {
		return null;
	}
	
	/**
	 * shutdown client connection 
	 */
	public void shutdown() {
		if (socket != null && socket.isConnected()) {
			try {
				socket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}
	
	

}
