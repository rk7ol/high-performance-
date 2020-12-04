package HighPerformanceHC;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

public class MainApp extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception {


        FXMLLoader fxmlLoader = new FXMLLoader();

        fxmlLoader.setLocation(MainApp.class.getResource("/views/MainPage.fxml"));

        BorderPane pane = fxmlLoader.load();

        Scene scene = new Scene(pane);

        primaryStage.setScene(scene);

        primaryStage.setTitle("high performance computing course design");
        primaryStage.setOnCloseRequest(event -> {
            Platform.exit();
            System.exit(0);
        });



        primaryStage.show();


    }

    public static void main(String[] args) {
        launch(args);
    }


}
