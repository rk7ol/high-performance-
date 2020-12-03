package HighPerformanceHC.utils;

import java.util.LinkedList;
import java.util.List;

public class MatrixGenerator {


    /**
     * @param function
     * @param ranges   x_max x_min y_max y_min z_max z_min
     * @return
     */
    public List<int[]> generate(FunctionMulArgs function, int size_x, int size_y, int size_z, int offset_x, int offset_y, int offset_z, double... ranges) {


        if (ranges.length != 6)
            throw new IllegalStateException("ranges size suppose to be 6");


        //ini

        List<int[]> matrix = new LinkedList<>();
        int[] coord;
        double x;
        double y;
        double z;

        double x_max = Math.ceil(Math.max(ranges[0], ranges[1]));
        double x_min = Math.floor(Math.min(ranges[0], ranges[1]));
//        double x_max = Math.max(ranges[0], ranges[1]);
//        double x_min = Math.min(ranges[0], ranges[1]);

        double step_x = (x_max - x_min) / size_x;

        double y_max = (int) Math.ceil(Math.max(ranges[2], ranges[3]));
        double y_min = (int) Math.floor(Math.min(ranges[2], ranges[3]));

//        double y_max = Math.max(ranges[2], ranges[3]);
//        double y_min = Math.min(ranges[2], ranges[3]);
        double step_y = (y_max - y_min) / size_y;

        double z_max = (int) Math.ceil(Math.max(ranges[4], ranges[5]));
        double z_min = (int) Math.floor(Math.min(ranges[4], ranges[5]));

//        double z_max = Math.max(ranges[4], ranges[5]);
//        double z_min = Math.min(ranges[4], ranges[5]);
        double step_z = (z_max - z_min) / size_z;


        for (int i = 0; i < size_x; i++) {

            for (int j = 0; j < size_y; j++) {

                x = x_min + step_x * i;
                y = y_min + step_y * j;

                z = function.fun(x, y);

                if (!Double.isNaN(z)) {
                    coord = new int[3];

                    coord[0] = (int) Math.round((x - x_min) / (x_max - x_min) * size_x) + offset_x;
                    coord[1] = (int) Math.round((y - y_min) / (y_max - y_min) * size_y) + offset_y;
                    coord[2] = (int) Math.round((z - z_min) / (z_max - z_min) * size_z) + offset_z;


                    //out of range
                    if (coord[0] == size_x || coord[1] == size_y || coord[2] == size_z)
                        continue;


                    matrix.add(coord);

                }
            }
        }

        return matrix;


    }


//    public static List<int[]> GetMatrix(FunctionMulArgs FunctionMulArgs, int size_x, int size_y, int size_z, int offset_x, int offset_y, int offset_z){
//
//
//
//        BorderFinder borderFinder = new BorderFinder();
//
//        double[] border = borderFinder.find(FunctionMulArgs);
//
//        Range range_x = new Range((float)border[0], (float)border[1]);
//        Range range_y = new Range((float)border[2], (float)border[3]);
//
//        System.out.println("x max:" + range_x.getMax() + ",x min:" + range_x.getMin());
//
//        System.out.println("y max:" + range_y.getMax() + ",y min:" + range_y.getMin());
//
//        GeneticAlgorithm GA = new GeneticAlgorithm(2000,0.01, FunctionMulArgs, range_x.getMin(), range_x.getMax(), range_y.getMin(), range_y.getMax());
//
//        System.out.println("z max:" + GA.play(GeneticAlgorithm.SelectType.GETMAX));
//
//        double z_max = GA.play(GeneticAlgorithm.SelectType.GETMAX);
//
//        double z_min = GA.play(GeneticAlgorithm.SelectType.GETMIN);
//
//
//        double[] ranges = new double[6];
//
//        System.arraycopy(border, 0, ranges, 0, border.length);
//
//        ranges[4] = z_max;
//
//        ranges[5] = z_min;
//
//
//        MatrixGenerator matrixGenerator = new MatrixGenerator();
//
//
//        List<int[]> matrix = matrixGenerator.generate(FunctionMulArgs, size_x, size_y, size_z, offset_x, offset_y, offset_z, ranges);
//
//
//        FileIO fileIO = new FileIO();
//
//        fileIO.writeMatrix(matrix, "data_fixed02", size_x, size_y, size_z);
//
//
//
//        return matrix;
//
//
//    }


}
