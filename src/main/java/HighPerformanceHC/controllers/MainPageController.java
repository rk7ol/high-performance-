package HighPerformanceHC.controllers;

import HighPerformanceHC.communicator.AvroDeserializer;
import HighPerformanceHC.communicator.DataConsumer;
import HighPerformanceHC.heat_conduct.ValueGenerator;
import HighPerformanceHC.native_implement.Invoker;
import HighPerformanceHC.utils.FunctionMulArgs;
import HighPerformanceHC.views.ChartFactory;
import HighPerformanceHC.views.HeatConductChart;
import javafx.application.Platform;
import javafx.beans.Observable;
import javafx.collections.FXCollections;
import javafx.collections.ObservableArray;
import javafx.collections.ObservableList;
import javafx.collections.ObservableListBase;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.Pane;
import javafx.util.Callback;
import javafx.util.converter.BigIntegerStringConverter;
import javafx.util.converter.NumberStringConverter;
import org.apache.avro.Schema;
import org.apache.avro.generic.GenericRecord;
import org.jzy3d.colors.Color;
import org.jzy3d.maths.Coord3d;

import java.io.IOException;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.function.UnaryOperator;


public class MainPageController {
    
    @FXML
    public Pane ViewChartPane;
    
    @FXML
    public Pane ControlChartPane;

    
    @FXML
    public ChoiceBox<Function> FunctionBox;

    @FXML
    public TextField NodeNumberTextField;


    private HeatConductChart viewChart;

    private final List<Coord3d> list = new LinkedList<>();
    private final List<Double> temperatures = new LinkedList<>();
    private final List<Color> colors = new LinkedList<>();


    private int x = 8;
    private int y = 8;
    private int z = 8;



    DataConsumer consumer = null;



    private final ObservableList<Function> functions = FXCollections.observableArrayList();




    private class Function{
        public Function(List<FunctionMulArgs> functionExpression, String name) {
            this.functionExpression = functionExpression;
            this.name = name;
        }
        List<FunctionMulArgs> functionExpression;
        String name;

        @Override
        public String toString() {
            return name;
        }
    }




    private void setFunctions(){

        List<FunctionMulArgs> sphere = new LinkedList<>();

        sphere.add(args -> Math.sqrt(1 - args[0] * args[0] - args[1] * args[1]) + 1);
        sphere.add(args -> -Math.sqrt(1 - args[0] * args[0] - args[1] * args[1]));

        functions.add(new Function(sphere, "sphere"));



    }









    private final Runnable updateDataJob = new Runnable() {
        @Override
        public void run() {

            viewChart.updateData(list, colors);
            list.clear();
            temperatures.clear();
            colors.clear();

        }
    };

    public Runnable receiveJob = () -> {


        double max_temperature;
        double min_temperature;

        double env = 25;

        long round = 0;


        System.out.println("receive start");


        while (true) {
            List<GenericRecord> result = consumer.receive();

            if (result.size() != 0) {

                for (GenericRecord record : result) {

                    if (round == (long) record.get("round")) {

                        //add
                        list.add(new Coord3d(((Integer) record.get("coord_x")).floatValue(), ((Integer) record.get("coord_y")).floatValue(), ((Integer) record.get("coord_z")).floatValue()));

                        temperatures.add((double) record.get("temp"));
                    } else {
                        //show

                        round = (long) record.get("round");

                        System.out.println("update:" + list.size());
                        System.out.println("round:" + round);

                        max_temperature = 500;


                        for (double temperature : temperatures) {
                            double percent = temperature / max_temperature;
                            colors.add(new Color((float) percent, (float) 0.0, (float) (1 - percent), (float) 1));
                        }

                        Platform.runLater(updateDataJob                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             );


                        try {
                            Thread.sleep(100);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }


                        list.add(new Coord3d(((Integer) record.get("coord_x")).floatValue(), ((Integer) record.get("coord_y")).floatValue(), ((Integer) record.get("coord_z")).floatValue()));
                        temperatures.add((double) record.get("temp"));


                    }



                }


            }
        }


    };





    private final Runnable computingJob = new Runnable() {
        @Override
        public void run() {

            System.out.println("start calculate");
            Function function;
            int nodeNumber;
            try {
                function = FunctionBox.getValue();
                nodeNumber = Integer.parseInt(NodeNumberTextField.getText());
            }catch (Exception e){
                System.out.println("value invalid");
                return;
            }

            System.out.println("function:" + function);
            System.out.println("node number:" + nodeNumber);

            ValueGenerator valueGenerator = new ValueGenerator();
            int[][][] result = valueGenerator.generate(function.functionExpression, x, y, z, -1, 1, -1, 1);


            new Invoker().InvokeMPIHeatConduct("HP", x, y, z, result, nodeNumber);

            System.out.println(result.length);

        }
    };







    @FXML
    public void receiveEvent() throws IOException {

        if (consumer != null) {
            return;
        }

        Schema schema = new Schema.Parser().parse(MainPageController.class.getResourceAsStream("/Point.avsc"));

        AvroDeserializer deserializer = new AvroDeserializer(schema);

        consumer = new DataConsumer(deserializer);

        consumer.subscribe(Collections.singletonList("test"));

        Thread thread = new Thread(receiveJob);

        thread.start();

    }






    @FXML
    public void startEvent(){

        Thread thread = new Thread(computingJob);


        thread.start();



    }




    @FXML
    public void initialize() {

        setFunctions();

        FunctionBox.setItems(functions);

        FunctionBox.setValue(functions.get(0));


        NodeNumberTextField.setTextFormatter(new TextFormatter<>(new BigIntegerStringConverter()));

        NodeNumberTextField.setOnKeyTyped(keyEvent -> NodeNumberTextField.commitValue());










        if (viewChart == null) {

            viewChart = ChartFactory.GetScatterChart(list, colors);
            Platform.runLater(new Runnable() {
                @Override
                public void run() {
                    ChartFactory.BindPane(viewChart, ViewChartPane);
                    viewChart.getAWTView().updateBounds();
                }
            });

        }




    }


}
