package org.krieger.beerbot.client;

import java.io.IOException;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.stage.Stage;

/**
 * base beer bot client application 
 * 
 * @author tkrieger
 *
 */
public class MainApp extends Application {
	
	/**
	 * beerbot network client 
	 */
	private BeerNetClient client;
	
	@FXML
	private Circle statLight;
	
	@FXML
	private Label statusLabel;
	
	@FXML 
	private Label distLabel;
	
	@FXML
	private TextField hostnameField;
	
	@FXML
	private ListView<String> receiverList;
	
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
	
	/**
	 * open a beerbot client network connection 
	 * @param e
	 */
	@FXML
	protected void openConnection(ActionEvent ae) {
		
		// check existing connection 
		if (client != null) {
			Alert alert = new Alert(AlertType.WARNING);
			alert.setTitle("Beer Bot");
			alert.setHeaderText(null);
			alert.setContentText("Bitte schließe erst die Verbindung zu dem aktuellen BeerBot!");

			alert.showAndWait();					
			return;
		}
		
		// check hostname input
		String hostname = hostnameField.getText();
		if (hostname == null || hostname.trim().isEmpty()) {
			Alert alert = new Alert(AlertType.WARNING);
			alert.setTitle("Beer Bot");
			alert.setHeaderText(null);
			alert.setContentText("Bitte gib zuerst den Hostname / IP für den BeerBot ein!");

			alert.showAndWait();	
			return;
		}
		
		try {
			client = new BeerNetClient(hostname);
			new Thread(() -> {
				try {
					System.out.println("connect...");
					client.connect();
					Platform.runLater(() -> {
						statLight.setFill(Color.GREEN);						
					});
				} catch (Exception e) {
					e.printStackTrace();
					Platform.runLater(() -> {
						statLight.setFill(Color.RED);
						statusLabel.setText("Fehler beim Verbinden: " + e.getMessage());						
					});
					client = null;
				}
			}).start();
		} catch (Exception e) {
			e.printStackTrace();
			statLight.setFill(Color.RED);
			statusLabel.setText("Fehler beim Verbinden: " + e.getMessage());
			client = null;
		}
	}
	
	/**
	 * close connection and shutdown the beerbot 
	 * 
	 * @param e
	 */
	@FXML
	protected void clientShutdown(ActionEvent ae) {
		System.out.println("shutdown client");
		try {
			client.shutdown();
			client = null;
			statLight.setFill(Color.DARKGRAY);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * util method to check connection state 
	 * 
	 * @return
	 */
	private boolean checkBotConnection() {
		
		if (client == null || !client.isConnected()) {
			Alert alert = new Alert(AlertType.WARNING);
			alert.setTitle("BeerBot");
			alert.setHeaderText(null);
			alert.setContentText("Bitte verbinden Sie sich zuerst mit dem BeerBot!");

			alert.showAndWait();
			
			client = null;
			
			return false;
		}

		return true;
	}
	
	/**
	 * UI Action to execute a short distance measurement
	 * 
	 * @param ae
	 */
	@FXML
	protected void measureDistance(ActionEvent ae) {
		System.out.println("pull distance");
		
		if (!checkBotConnection()) return;
		
		new Thread(() -> {
			try {
				Double dist = client.pullDistance();
				Platform.runLater(() -> {
					distLabel.setText(String.format("%.2f", dist));
				});
			} catch (IOException e) {
				e.printStackTrace();
			}
		}).start();
	}
	
	/**
	 * send a ui move bot command to the roboter
	 * 
	 * @param ae
	 */
	@FXML
	protected void moveBot(ActionEvent ae) {
		System.out.println(ae.getSource());
		
		if (!checkBotConnection()) {
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
					client.moveCommand(dir);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}).start();
		}
	}
	
	/**
	 * UI Action to start a beer delivery
	 * 
	 * @param ae
	 */
	@FXML
	protected void orderBeer(ActionEvent ae) {		
		
		String receiver = receiverList.getSelectionModel().getSelectedItem();
		
		if (receiver == null) {
			Alert alert = new Alert(AlertType.WARNING);
			alert.setTitle("Bier bestellen");
			alert.setHeaderText(null);
			alert.setContentText("Bitte wählen Sie zuerst einen Empfänger für das leckere Bier!");

			alert.showAndWait();		
		} else {
			System.out.println("order beer: " + receiver);			
		}
	}
	
	

	/**
	 * javafx application start 
	 */
	@Override
	public void start(Stage stage) {
		Parent root;
		try {
			root = FXMLLoader.load(getClass().getResource("/MainApp.fxml"));
			Scene scene = new Scene(root, 800, 600);

			stage.setTitle("BeerBot Client");
			stage.setScene(scene);
			stage.show();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * start client program 
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
	    launch();
	}

}
