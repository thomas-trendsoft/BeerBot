package org.krieger.beerbot.client;

import java.io.IOException;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;

public class ScannerWidget {
	
	private MainApp controller;
	
	@FXML 
	private Label distLabel;
	
	@FXML
	private Button btMeasure;
	
	@FXML
	private Button btCalibration;

	public void init(MainApp controller) {
		this.controller = controller;
	}
	
	/**
	 * UI Action to execute a short distance measurement
	 * 
	 * @param ae
	 */
	@FXML
	protected void measureDistance(ActionEvent ae) {
		System.out.println("pull distance");
		
		if (!controller.checkBotConnection()) return;
		
		btMeasure.setDisable(true);
		new Thread(() -> {
			try {
				Double dist = controller.client.pullDistance();
				Platform.runLater(() -> {
					distLabel.setText(String.format("%.2f cm", dist));
					btMeasure.setDisable(false);
				});
			} catch (IOException e) {
				e.printStackTrace();
			}
		}).start();
	}
	
	@FXML
	protected void eyeCalibration(ActionEvent ae) {
		System.out.println("start eye calibration");
		
		if (!controller.checkBotConnection()) return;

		btCalibration.setDisable(true);
		new Thread(() -> {
			try {
				controller.client.eyeCalibration();
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				Platform.runLater(() -> {
					btCalibration.setDisable(false);
				});
			}
		}).start();
	}
	
}
