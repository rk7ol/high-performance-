package HighPerformanceHC.heat_conduct;

import HighPerformanceHC.utils.BorderFinder;
import HighPerformanceHC.utils.FunctionMulArgs;
import HighPerformanceHC.utils.GeneticAlgorithm;
import HighPerformanceHC.utils.MatrixGenerator;

import java.util.LinkedList;
import java.util.List;

public class ValueGenerator {

    private static final int POPULATION_SIZE = 2000;
    private static final double GENE_MUTATION_RATE = 0.01;


//    public int[][][] generate(List<FunctionMulArgs> functionList){
//
//
//
//
//
//
//
//    }

    /**
     * with specific range
     *
     * @param functionList board function list
     * @param range        variable range, max x, min x, max y, min y
     * @return scatter
     */
    public int[][][] generate(List<FunctionMulArgs> functionList, int x_size, int y_size, int z_size, double... range) {
        //public List<int[]> generate(List<FunctionMulArgs> functionList, int x_size, int y_size, int z_size, double ... range){

        if (range.length != 4)
            throw new IllegalStateException("ranges size suppose to be 4");


        int[][][] result = new int[x_size][y_size][z_size];


        double z_max = Double.MIN_VALUE;
        double z_min = Double.MAX_VALUE;


        for (FunctionMulArgs function : functionList) {
            z_max = Math.max(getExtremeValue(function, GeneticAlgorithm.SelectType.GETMAX, range), z_max);
            z_min = Math.min(getExtremeValue(function, GeneticAlgorithm.SelectType.GETMIN, range), z_min);
        }

        z_max++;
        z_min--;

        List<int[]> borderList = new LinkedList<>();


        //get border
        MatrixGenerator matrixGenerator = new MatrixGenerator();
        for (FunctionMulArgs function : functionList) {
            borderList.addAll(matrixGenerator.generate(function, x_size, y_size, z_size, 0, 0, 0, range[0], range[1], range[2], range[3], z_max, z_min));
        }

        //fill
        for (int[] border : borderList) {
            result[border[0]][border[1]][border[2]] = 1;
        }


        //return borderList;

        fillMatrixByBorder(result, x_size, y_size, z_size);

        return result;
    }

    /**
     * auto range
     *
     * @param functionList
     * @param x_size
     * @param y_size
     * @param z_size
     * @return
     */
    public int[][][] generate(List<FunctionMulArgs> functionList, int x_size, int y_size, int z_size) {

        BorderFinder borderFinder = new BorderFinder();

        double x_max = Double.MIN_VALUE;
        double x_min = Double.MAX_VALUE;

        double y_max = Double.MIN_VALUE;
        double y_min = Double.MAX_VALUE;


        for (FunctionMulArgs function : functionList) {
            double[] border = borderFinder.find(function);
            double x_maxOfFunction = Math.max(border[0], border[1]);
            double x_minOfFunction = Math.min(border[0], border[1]);

            x_max = Math.max(x_maxOfFunction, x_max);
            x_min = Math.min(x_minOfFunction, x_min);

            double y_maxOfFunction = Math.max(border[2], border[3]);
            double y_minOfFunction = Math.min(border[2], border[3]);

            y_max = Math.max(y_maxOfFunction, y_max);
            y_min = Math.min(y_minOfFunction, y_min);
        }

        return generate(functionList, x_size, y_size, z_size, x_max, x_min, y_max, y_min);
    }


    private double getExtremeValue(FunctionMulArgs function, GeneticAlgorithm.SelectType type, double... range) {

        return new GeneticAlgorithm(POPULATION_SIZE, GENE_MUTATION_RATE, function, range).play(type);
    }


    private int[][][] fillMatrixByBorder(int[][][] borderMatrix, int x_length, int y_length, int z_length) {


        boolean isIn = false;


        for (int i = 0; i < x_length; i++) {
            for (int j = 0; j < y_length; j++) {


                //pretreatment
                int count = 0;
                for (int k = 0; k < z_length; k++) {

                    if (borderMatrix[i][j][k] == 1)
                        count++;
                }
                if (count <= 1)
                    continue;


                //fill
                isIn = false;
                for (int k = 0; k < z_length; k++) {


                    if (borderMatrix[i][j][k] == 1) {
                        //border
                        isIn = !isIn;
                    } else {
                        if (isIn)
                            borderMatrix[i][j][k] = 1;
                    }
                }


            }
        }


        return borderMatrix;


    }

    public static void main(String[] args) {


        System.out.println("finish");
    }


}
