package HighPerformanceHC.views;

import org.jzy3d.chart.AWTChart;
import org.jzy3d.chart.factories.IChartComponentFactory;
import org.jzy3d.colors.Color;
import org.jzy3d.maths.Coord3d;
import org.jzy3d.plot3d.primitives.Scatter;
import org.jzy3d.plot3d.rendering.canvas.Quality;

import java.util.List;

public class HeatConductChart extends AWTChart {
    private Scatter data;


    public HeatConductChart(IChartComponentFactory factory, Quality quality, String windowingToolkit) {
        super(factory, quality, windowingToolkit);
        this.data = new Scatter();
    }

    public void updateData(List<Coord3d> points, List<Color> colors) {

        if (points.size() == 0 || colors.size() == 0)
            throw new IllegalStateException("point size suppose bigger than 0");

        Scatter new_data = new Scatter();

        new_data.setData(points.toArray(new Coord3d[0]));
        new_data.setColors(colors.toArray(new Color[0]));

        new_data.setWidth(3);

        this.getScene().getGraph().add(new_data);

        this.getScene().getGraph().remove(data);
        data = new_data;


        this.getView().updateBounds();

    }


    public void setData(Scatter data) {
        this.data = data;
        data.setWidth(3);
        this.getScene().getGraph().add(this.data, true);

    }


}
