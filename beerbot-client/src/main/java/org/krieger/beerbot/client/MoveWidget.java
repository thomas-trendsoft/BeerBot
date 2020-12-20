package org.krieger.beerbot.client;

import java.io.IOException;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;

public class MoveWidget {

	@FXML
	private Button btMoveUp;
	
	@FXML
	private Button btMoveDown;
	
	@FXML
	private Button btMoveLeft;
	
	@FXML
	private Button btMoveRight;
	
	@FXML
	private Button btMoveStop;

	private MainApp controller;

	public void init(MainApp controller) {
		this.controller = controller;
	}
	

	/**
	 * send a ui move bot command to the roboter
	 * 
	 * @param ae
	 */
	@FXML
	protected void moveBot(ActionEvent ae) {
		System.out.println(ae.getSource());
		
		if (!controller.checkBotConnection()) {
			return;
		}
		
		final Integer dir;
		if (ae.getSource() == btMoveStop) {
			dir = 0;
		} else if (ae.getSource() == btMoveUp) {
			dir = 1;
		} else if (ae.getSource() == btMoveDown) {
			dir = 2;
		} else if (ae.getSource() == btMoveLeft) {
			dir = 3;
		} else if (ae.getSource() == btMoveRight) {
			dir = 4;
		} else { 
			dir = -1;
		}
		
		System.out.println(dir);
		if (dir >= 0) {
			new Thread(() -> {
				try {
					controller.client.moveCommand(dir);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}).start();
		}
	}
	
}
