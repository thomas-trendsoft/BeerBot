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
	
	// move direction constants
	protected static String[] moveDirs = {"STOP","FORWARD","BACKWARD","LEFT","RIGHT"};
	
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
	 * flag if client is connected to bot
	 */
	private boolean connected;
	
	/**
	 * default constructor 
	 * 
	 * @param hostname
	 */
	public BeerNetClient(String hostname) {
		this.hostname = hostname;
		this.connected = false;
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
		input  = this.socket.getInputStream();
		
		Message msg = new Message();
		msg.put("PROST", "1");
		
		msg = queryData(msg);
		
		if (msg.containsKey("PROST")) {
			this.connected = true;			
		} else {
			System.out.println("no handshake received!");
			this.shutdown();
		}
				
	}
	
	public boolean isConnected() {
		return connected;
	}
	
	private synchronized Message queryData(Message q) throws IOException {
		this.sendMessage(q);
		return this.readMessage();
	}

	
	private void sendMessage(Message msg) throws IOException {
		int clen = 0;
		
		StringBuffer buf = new StringBuffer();

		for (String k : msg.keySet()) {
			clen += k.length() + 1;
			clen += msg.get(k).length() + 1;
			buf.append(k + ";");
			buf.append(msg.get(k) + ";");
		}
		
		buf.insert(0, "LEN;" + clen + ";");
		
		output.write(buf.toString().getBytes("utf-8"));
		
	}
	
	/**
	 * read a message response and parse to key/value map
	 * 
	 * @return
	 * @throws IOException
	 */
	private Message readMessage() throws IOException {
		int    rc;
		byte[] buf;
		
		StringBuffer  strbuf = new StringBuffer();
		Message       data   = new Message();
		
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
		int mlen = Integer.parseInt(msgstr.substring(4, lidx));
		// + 2 ; + \n 
		if (mlen != (msgstr.length() - (lidx+2))) {
			throw new IOException("bad response: len does not match " + mlen + "/" + msgstr.length() + "/" + lidx);
		}
		
		// parse parameter
		boolean key = true;
		lidx++;
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
			lidx = tidx+1;
			tidx = msgstr.indexOf(';',tidx+1);
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
		
		//HashMap<String,String> data = readMessage();
		
		return map;
	}
	
	
	public Double pullDistance() throws UnsupportedEncodingException, IOException {
		Message m = new Message();
		
		m.put("PULL", "DIST");
		Message data = queryData(m);
		System.out.println(data.get("RESULT"));
		
		double result = -1.0;
		if (data.containsKey("RESULT")) {
			try {
				result = Double.parseDouble(data.get("RESULT"));
			} catch (NumberFormatException nfe) {
				nfe.printStackTrace();
			}
		}
		
		return result;
	}
	
	public void eyeCalibration() throws IOException {
		Message m = new Message();
		
		m.put("CMD", "EYECAL");
		
		// read response
		Message resp = this.queryData(m);
		System.out.println(resp.get("RESULT"));
		
	}
	
	/**
	 * send simple move command 
	 * 
	 * - 0 = pause
	 * - 1 = forward
	 * - 2 = backward
	 * - 3 = left
	 * - 4 = right
	 * 
	 * @param dir
	 * @throws IOException 
	 */
	public void moveCommand(int midx) throws IOException {
		String dir = moveDirs[midx];

		Message m = new Message();
		m.put("CMD", "MOVE");
		m.put("DIR", dir);
		
		Message resp = queryData(m);
		System.out.println(resp.get("RESULT"));
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
		output.write(("LEN;" + len + ";ORDER;" + receiverid + "\n").getBytes("utf-8"));
		
		HashMap<String,String> data = readMessage();
		
		return data.get("MSG");
	}
	
	public String createMap() throws IOException {
		Message m = new Message();
		
		m.put("CMD", "STARTMAP");
		
		Message a = queryData(m);
		
		return a.get("RESULT");
	}

	/**
	 * pull the current robot position 
	 * 
	 * @return
	 * @throws UnsupportedEncodingException
	 * @throws IOException
	 */
	public Position pullStatus() throws UnsupportedEncodingException, IOException {
		double x = -9999.0,y = -9999.0,theta = -400.0;
		
		// send pull request
		Message m = new Message();
		m.put("PULL", "STAT");
		
		Message data = queryData(m);
		
		if (data.containsKey("X"))
			x = Double.parseDouble(data.get("X"));
		if (data.containsKey("Y"))
			y = Double.parseDouble(data.get("Y"));
		if (data.containsKey("THETA"))
			theta = Double.parseDouble(data.get("THETA"));
		
		return new Position(x, y, theta);
	}	
	
	/**
	 * shutdown client connection 
	 */
	public void shutdown() {
		Message msg = new Message();
		msg.put("CLOSE", "1");
		this.connected = false;
		if (socket != null && socket.isConnected()) {
			try {
				this.sendMessage(msg);
				socket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}
	
	

}
