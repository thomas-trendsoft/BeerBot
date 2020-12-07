package org.krieger.beerbot.client;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.HashMap;

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
	
	/**
	 * order a new beer for a given receiver 
	 * 
	 * @param receiverid
	 * @return
	 * @throws IOException 
	 * @throws UnsupportedEncodingException 
	 */
	public String orderBeer(String receiverid) throws UnsupportedEncodingException, IOException {
		int len = 7 + receiverid.length();
		
		// send pull request
		output.write(("LEN;9;ORDER;" + receiverid + "\n").getBytes("utf-8"));
		
		HashMap<String,String> data = readMessage();
		
		return data.get("MSG");
	}

	/**
	 * pull the current robot position 
	 * 
	 * @return
	 * @throws UnsupportedEncodingException
	 * @throws IOException
	 */
	public Position pullPosition() throws UnsupportedEncodingException, IOException {
		double x = -9999.0,y = -9999.0,theta = -400.0;
		
		// send pull request
		output.write("LEN;9;PULL;POS\n".getBytes("utf-8"));
		
		HashMap<String,String> data = readMessage();
		
		if (data.containsKey("X"))
			x = Double.parseDouble(data.get("x"));
		if (data.containsKey("Y"))
			y = Double.parseDouble(data.get("y"));
		if (data.containsKey("THETA"))
			theta = Double.parseDouble(data.get("theta"));
		
		return new Position(x, y, theta);
	}
	
	/**
	 * read a message response and parse to key/value map
	 * 
	 * @return
	 * @throws IOException
	 */
	private HashMap<String, String> readMessage() throws IOException {
		int    rc;
		byte[] buf;
		
		StringBuffer         strbuf = new StringBuffer();
		HashMap<String,String> data = new HashMap<>();
		
		// read message data and create a string
		buf = new byte[1024];
		while ((rc = input.read(buf, 0, 1024)) > 0) {
			strbuf.append(new String(buf,0,rc));
			if (buf[rc-1] == '\n')
				break;
		}
		
		String msgstr = strbuf.toString();
		
		// read len msg
		if (!msgstr.startsWith("LEN;")) {
			throw new IOException("bad response: no len prefix");
		}
		
		// check len str
		int lidx = msgstr.indexOf(';', 4);
		if (lidx < 0) {
			throw new IOException("bad response no len end found.");
		}
		
		// check len 
		System.out.println("check msg len: " + msgstr.substring(4,lidx));
		int mlen = Integer.parseInt(msgstr.substring(4, lidx));
		if (mlen != msgstr.length()) {
			throw new IOException("bad response: len does not match " + mlen + "/" + msgstr.length());
		}
		
		// parse parameter
		boolean key = true;
		int    tidx = msgstr.indexOf(';', lidx);
		
		String kstr = null,vstr = null;
		while (tidx > 0) {
			if (key) {
				kstr = msgstr.substring(lidx, tidx);
				key  = false;
			} else {
				vstr = msgstr.substring(lidx, tidx);
				key  = true;
				data.put(kstr, vstr);
			}
			lidx = tidx;
			tidx = msgstr.indexOf(';',tidx);
		}
		
		return data;
	}

	/**
	 * pull active map data
	 * 
	 * @return
	 * @throws IOException 
	 * @throws UnsupportedEncodingException 
	 */
	public Map pullMap() throws UnsupportedEncodingException, IOException {
		Map map = null;
		
		// send pull request
		output.write("LEN;9;PULL;MAP\n".getBytes("utf-8"));
		
		HashMap<String,String> data = readMessage();
		
		return map;
	}
	
	/**
	 * pull the current status message
	 * 
	 * @return
	 * @throws UnsupportedEncodingException 
	 * @throws IOException
	 */
	public String pullStatus() throws UnsupportedEncodingException, IOException {
		// send pull request
		output.write("LEN;10;PULL;STAT\n".getBytes("utf-8"));
		
		HashMap<String,String> data = readMessage();
		
		return data.get("STATUS");
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
