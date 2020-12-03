package HighPerformanceHC.views;

import javafx.scene.Node;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Pane;
import org.jzy3d.chart.Chart;
import org.jzy3d.chart.controllers.keyboard.camera.ICameraKeyController;
import org.jzy3d.chart.factories.IChartComponentFactory;
import org.jzy3d.colors.Color;
import org.jzy3d.javafx.JavaFXChartFactory;
import org.jzy3d.javafx.controllers.keyboard.JavaFXCameraKeyController;
import org.jzy3d.maths.Coord3d;
import org.jzy3d.plot3d.primitives.Scatter;
import org.jzy3d.plot3d.rendering.canvas.Quality;

import java.util.List;

public class ChartFactory {

    private static final NonKeyControllerChartFactory NonKeyControllerChartFactory = new NonKeyControllerChartFactory();

    public static HeatConductChart GetScatterChart(List<Coord3d> points, List<Color> colors) {


        HeatConductChart scatterChart = (HeatConductChart) NonKeyControllerChartFactory.newChart(Quality.Advanced, IChartComponentFactory.Toolkit.offscreen);
        Scatter scatter;
        if (colors != null) {
            scatter = new Scatter(points.toArray(new Coord3d[0]), colors.toArray(new Color[0]));
        } else {
            scatter = new Scatter(points.toArray(new Coord3d[0]));

        }

        scatter.setWidth(2);

        scatterChart.setData(scatter);


        return scatterChart;

    }

    public static HeatConductChart GetScatterChart(List<Coord3d> points) {


        return GetScatterChart(points, null);


    }


    public static void BindPane(HeatConductChart chart, Pane pane) {


        ImageView imageView = NonKeyControllerChartFactory.bindImageView(chart);

        ((JavaFXCameraKeyController) chart.addKeyboardCameraController()).setNode(imageView);

        imageView.fitHeightProperty().bind(pane.heightProperty());
        imageView.fitWidthProperty().bind(pane.widthProperty());


        NonKeyControllerChartFactory.addSceneSizeChangedListener(chart, pane.getScene());

        pane.getChildren().add(imageView);


    }


}


class NonKeyControllerChartFactory extends JavaFXChartFactory {


    class NoRegisterKeyController extends JavaFXCameraKeyController {

        public NoRegisterKeyController(Chart chart, Node node) {
            super(chart, node);
        }


        @Override
        public void register(Chart chart) {
            //do nothing
        }
    }

    @Override
    public Chart newChart(IChartComponentFactory factory, Quality quality, String toolkit) {
        return new HeatConductChart(factory, quality, toolkit);
    }


    @Override
    public ICameraKeyController newKeyboardCameraController(Chart chart) {
        return new NoRegisterKeyController(chart, null);
    }
}
