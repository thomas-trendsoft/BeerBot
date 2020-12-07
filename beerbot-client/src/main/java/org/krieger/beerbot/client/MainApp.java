package org.krieger.beerbot.client;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
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
	
	/**
	 * open a beerbot client network connection 
	 * @param e
	 */
	@FXML
	protected void openConnection(ActionEvent e) {
		System.out.println("open con");
	}

	/**
	 * javafx application start 
	 */
	@Override
	public void start(Stage stage) {
		Parent root;
		try {
			root = FXMLLoader.load(getClass().getResource("/MainApp.fxml"));
			Scene scene = new Scene(root, 600, 600);

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
