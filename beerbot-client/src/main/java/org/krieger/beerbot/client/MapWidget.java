package org.krieger.beerbot.client;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

/**
 * controller for the map view 
 * 
 * @author tkrieger
 *
 */
public class MapWidget {
	
	private MainApp controller;

	public void init(MainApp controller) {
		this.controller = controller;
	}

	public void updateMap() {
		System.out.println("update map");
		
		if (!controller.checkBotConnection()) {
			return;
		}

		try {
			controller.client.pullMap();
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	

}
