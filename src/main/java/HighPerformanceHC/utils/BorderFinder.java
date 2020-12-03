package HighPerformanceHC.utils;

import java.util.Arrays;

public class BorderFinder {

    public double[] find(FunctionMulArgs function) {

        double x_max;
        double x_min;

        double y_max;
        double y_min;

        double x_cursor;

        double y_cursor;

        double step = 0.1;

        double precision = 0.01;

        double beforeChange;
        double change;

        int count = 0;


        //find ini point

        loop:
        while (true) {

            count++;

            for (int i = 0; i < count; i++) {

                for (int j = 0; j < count; j++) {

                    x_cursor = step * i - step * count / 2;
                    y_cursor = step * j - step * count / 2;

                    if (!Double.isNaN(function.fun(x_cursor, y_cursor))) {

                        x_max = x_cursor;
                        x_min = x_cursor;

                        y_max = y_cursor;
                        y_min = y_cursor;
                        break loop;
                    }
                }
            }

        }

        //find board

        do {

            change = 0;
            //for x

            //for x max
            beforeChange = x_max;
            x_max = extremeValue(x_max, ExtremeValueType.MAX, ValueDimension.X, function, new double[]{y_min, y_max}, step, precision);
            change = Math.max(Math.abs(beforeChange - x_max), change);

            //for x min
            beforeChange = x_min;
            x_min = extremeValue(x_min, ExtremeValueType.MIN, ValueDimension.X, function, new double[]{y_min, y_max}, step, precision);
            change = Math.max(Math.abs(beforeChange - x_min), change);

            //for y

            //for y max
            beforeChange = y_max;
            y_max = extremeValue(y_max, ExtremeValueType.MAX, ValueDimension.Y, function, new double[]{x_min, x_max}, step, precision);
            change = Math.max(Math.abs(beforeChange - y_max), change);


            //for y min
            beforeChange = y_min;
            y_min = extremeValue(y_min, ExtremeValueType.MIN, ValueDimension.Y, function, new double[]{x_min, x_max}, step, precision);
            change = Math.max(Math.abs(beforeChange - y_min), change);


        } while (change > precision);

        return new double[]{x_min, x_max, y_min, y_max};


    }

    private enum ExtremeValueType {

        MAX, MIN

    }

    private enum ValueDimension {
        X, Y
    }


    private double extremeValue(double value, ExtremeValueType type, ValueDimension dimension, FunctionMulArgs function, double[] changeRange, double step, double precision) {
        double cursor = value;

        int count;

        for (double x = Arrays.stream(changeRange).min().getAsDouble(); x <= Arrays.stream(changeRange).max().getAsDouble(); x += step) {

            count = 1;
            loop:
            do {

                switch (type) {
                    case MAX:
                        cursor += count * step;
                        break;
                    case MIN:
                        cursor -= count * step;
                        break;
                    default:
                        System.err.println("unknown type");
                }

                switch (dimension) {
                    case X:

                        if (Double.isNaN(function.fun(cursor, x))) {
                            count = 1;
                            while (Double.isNaN(function.fun(cursor, x))) {
                                cursor = (cursor + value) / 2;
                                if (Math.abs(cursor - value) < precision)
                                    break loop;

                            }
                        }

                        break;

                    case Y:

                        if (Double.isNaN(function.fun(x, cursor))) {
                            count = 1;
                            while (Double.isNaN(function.fun(x, cursor))) {
                                cursor = (cursor + value) / 2;
                                if (Math.abs(cursor - value) < precision)
                                    break loop;

                            }
                        }

                        break;
                    default:
                        System.err.println("unknown type");

                }


                if (Math.abs(cursor - value) < precision) {
                    switch (type) {
                        case MAX:
                            value = Math.max(cursor, value);
                            break;
                        case MIN:
                            value = Math.min(cursor, value);
                            break;
                        default:
                            System.err.println("unknown type");
                    }

                    break;
                } else {
                    count *= 2;
                    switch (type) {
                        case MAX:
                            value = Math.max(cursor, value);
                            break;
                        case MIN:
                            value = Math.min(cursor, value);
                            break;
                        default:
                            System.err.println("unknown type");
                    }
                }


            } while (true);

        }

        return value;

    }


}
