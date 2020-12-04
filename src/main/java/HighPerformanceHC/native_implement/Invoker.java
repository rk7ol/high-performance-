package HighPerformanceHC.native_implement;


import HighPerformanceHC.heat_conduct.ValueGenerator;
import HighPerformanceHC.utils.FunctionMulArgs;

import java.util.LinkedList;
import java.util.List;

public class Invoker {

    static {
        System.loadLibrary("HP");
        //System.out.println("load:" + Invoker.class.getClassLoader().getResource("libHP.so").getPath());
        // System.load(Invoker.class.getClassLoader().getResource("libHP.so").getPath());
    }


    public native void InvokeMPIHeatConduct(String targetPath, int x_size, int y_size, int z_size, int[][][] calculateValue, int node_number);

    public static void main(String[] args) {
        System.out.println("start");
        //System.out.println("exec file path:" + Invoker.class.getClassLoader().getResource("main.c").getPath());

        int x = 8;
        int y = 8;
        int z = 8;

        ValueGenerator valueGenerator = new ValueGenerator();

        List<FunctionMulArgs> functionMulArgs = new LinkedList<>();


//        functionMulArgs.add(args1 ->  Math.sqrt(1 - args1[0] * args1[0] - args1[1] * args1[1]) + 5);
//        functionMulArgs.add(args1 ->  -Math.sqrt(1 - args1[0] * args1[0] - args1[1] * args1[1]));
//        int[][][] result = valueGenerator.generate(functionMulArgs, x, y, z);


        functionMulArgs.add(args1 -> Math.sqrt(1 - args1[0] * args1[0] - args1[1] * args1[1]) + 1);
        functionMulArgs.add(args1 -> -Math.sqrt(1 - args1[0] * args1[0] - args1[1] * args1[1]));
        int[][][] result = valueGenerator.generate(functionMulArgs, x, y, z, -1, 1, -1, 1);

//        functionMulArgs.add(args1 ->  Math.sin(args1[0]) + Math.cos(args1[1]) + 1);
//        functionMulArgs.add(args1 ->  -Math.sin(args1[0]) + Math.cos(args1[1]) + 1);
//        int[][][] result = valueGenerator.generate(functionMulArgs, x, y, z, -1, 1, -1, 1);


        System.out.println("result size:" + result.length);


        new Invoker().InvokeMPIHeatConduct("HP", x, y, z, result, 4);
        System.out.println("finish");
    }


}
