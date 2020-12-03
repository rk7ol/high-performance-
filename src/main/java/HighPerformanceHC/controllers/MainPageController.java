package HighPerformanceHC.controllers;

import HighPerformanceHC.communicator.AvroDeserializer;
import HighPerformanceHC.communicator.DataConsumer;
import HighPerformanceHC.heat_conduct.ValueGenerator;
import HighPerformanceHC.utils.FunctionMulArgs;
import HighPerformanceHC.views.ChartFactory;
import HighPerformanceHC.views.HeatConductChart;
import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.Slider;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Pane;
import org.apache.avro.Schema;
import org.apache.avro.generic.GenericRecord;
import org.jzy3d.colors.Color;
import org.jzy3d.maths.Coord3d;

import java.io.IOException;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;


public class MainPageController {


    @FXML
    public ImageView imageView;


    @FXML
    public Pane viewPane;


    public ImageView viewImageView;


    @FXML
    public Pane controlPane;

    public ImageView controlImageView;

    @FXML
    public Slider colorSlider;


    private HeatConductChart viewChart;

    private final List<Coord3d> list = new LinkedList<>();
    private final List<Double> temperatures = new LinkedList<>();
    private final List<Color> colors = new LinkedList<>();


    @FXML
    public void OnColorSliderChange() {

        double percent = colorSlider.getValue() / (colorSlider.getMax() - colorSlider.getMin());

        String R = Integer.toHexString((int) (percent * 255));
        String G = Integer.toHexString(00);
        String B = Integer.toHexString((int) ((1 - percent) * 255));


        if (R.length() == 1) {
            R = "0" + R;
        }

        if (B.length() == 1) {
            B = "0" + B;
        }

        String color = String.format("-fx-background-color : rgb(%d,%d,%d)", (int) (percent * 255), 0, (int) ((1 - percent) * 255));

        controlPane.setStyle(color);

    }


    DataConsumer consumer = null;


    private final Runnable updateDataJob = new Runnable() {
        @Override
        public void run() {

            viewChart.updateData(list, colors);
            list.clear();
            temperatures.clear();
            colors.clear();

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


    @FXML
    public void initialize() {

        if (viewChart == null) {

            viewChart = ChartFactory.GetScatterChart(list, colors);
            Platform.runLater(new Runnable() {
                @Override
                public void run() {
                    ChartFactory.BindPane(viewChart, viewPane);
                    viewChart.getAWTView().updateBounds();
                }
            });

        }


    }


    public ImageView getImageView() {
        return imageView;
    }
}
