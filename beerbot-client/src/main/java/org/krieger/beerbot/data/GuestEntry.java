package org.krieger.beerbot.data;

/**
 * guest list entry 
 * 
 * @author tkrieger
 *
 */
public class GuestEntry {
	
	/**
	 * bluetooth mac address
	 */
	private String mac;
	
	/**
	 * device name
	 */
	private String name;
	
	/**
	 * default constructor 
	 * 
	 * @param name
	 * @param mac
	 */
	public GuestEntry(String name,String mac) {
		this.mac  = mac;
		this.name = name;
	}

	public String getMac() {
		return mac;
	}

	public void setMac(String mac) {
		this.mac = mac;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
}
