package org.krieger.beerbot.client;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
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
	private TextField hostnameField;
	
	@FXML
	private ListView<String> receiverList;
	
	/**
	 * open a beerbot client network connection 
	 * @param e
	 */
	@FXML
	protected void openConnection(ActionEvent ae) {
		System.out.println("open con");
		
		// check existing connection 
		if (client != null) {
			Alert alert = new Alert(AlertType.WARNING);
			alert.setTitle("Bier Bot");
			alert.setHeaderText(null);
			alert.setContentText("Bitte schließe erst die Verbindung zu dem aktuellen BeerBot!");

			alert.showAndWait();					
			return;
		}
		
		// check hostname input
		String hostname = hostnameField.getText();
		if (hostname == null || hostname.trim().isEmpty()) {
			Alert alert = new Alert(AlertType.WARNING);
			alert.setTitle("Bier Bot");
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
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
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
