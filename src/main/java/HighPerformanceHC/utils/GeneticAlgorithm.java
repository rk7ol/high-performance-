package HighPerformanceHC.utils;


import HighPerformanceHC.heat_conduct.ValueGenerator;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.function.ToDoubleFunction;

public class GeneticAlgorithm {

    /**
     * GETMAX
     */
    public enum SelectType {
        GETMAX,
        GETMIN
    }

    //Population size
    private final int size;

    //Probability of gene mutation
    private final double probabilityOfGeneMutation;

    private final FunctionMulArgs function;

    private SelectType selectType;


    //[arg1max, arg2max, arg3max ...]
    private final double[] performanceMax;

    //[arg1min, arg2min, arg3min ...]
    private final double[] performanceMin;

    private final int geneLength = 32;
    private final int geneNum;

    private final int geneTotalLength;

    //max generation
    private final int generationLimits = 20;


    private Unit[] units;


    public GeneticAlgorithm(int size, double probabilityOfGeneMutation, FunctionMulArgs function, double... range) {
        this.size = size;
        this.probabilityOfGeneMutation = probabilityOfGeneMutation;
        this.function = function;

        performanceMin = new double[range.length / 2];
        performanceMax = new double[range.length / 2];

        geneNum = range.length / 2;
        geneTotalLength = geneLength * geneNum;


        for (int i = 0; i < range.length / 2; i++) {

            performanceMin[i] = Math.min(range[i], range[i + 1]);
            performanceMax[i] = Math.max(range[i], range[i + 1]);

        }


    }


    private class Unit {

        public byte[] gene;

        public Unit(double[] performance) {
            gene = performanceToGene(performance);
        }

        public Unit(byte[] gene) {
            this.gene = gene;
        }


        public double[] getPerformance() {
            return geneToPerformance(gene);
        }


        public double getPhenotype() {

            return function.fun(geneToPerformance(gene));

        }


        private double[] geneToPerformance(byte[] gene) {

            long bits = 0;
            double[] performances = new double[geneNum];


            //num of gene
            for (int i = 0; i < geneNum; i++) {
                bits = 0;
                //performance of gene
                for (int j = 0; j < geneLength; j++) {
                    bits += gene[j + i * geneLength] * Math.pow(2, j);
                }
                performances[i] = bits * (performanceMax[i] - performanceMin[i]) / Math.pow(2, geneLength) + performanceMin[i];

            }


            return performances;
        }


        private byte[] performanceToGene(double[] performance) {
            byte[] gene = new byte[geneTotalLength];

            long bits;

            for (int i = 0; i < performance.length; i++) {

                bits = (long) ((performance[i] - performanceMin[i]) / (performanceMax[i] - performanceMin[i]) * Math.pow(2, geneLength));

                for (int j = 0; j < geneLength; j++) {
                    gene[j + i * geneLength] = (byte) (bits % 2);
                    bits /= 2;
                }


            }


            return gene;
        }


        @Override
        public String toString() {
            return "Unit{" +
                    "gene=" + Arrays.toString(gene) +
                    ", performance=" + Arrays.toString(getPerformance()) +
                    '}';
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Unit unit = (Unit) o;
            return Arrays.equals(gene, unit.gene);
        }

        @Override
        public int hashCode() {
            return Arrays.hashCode(gene);
        }
    }


    private Unit[] generate() {

        Unit[] units;

        units = new Unit[size];

        //size of population
        for (int i = 0; i < size; i++) {

            double[] performance = new double[geneNum];

            do {

                for (int j = 0; j < geneNum; j++) {

                    performance[j] = Math.random() * (performanceMax[j] - performanceMin[j]) + performanceMin[j];
                }

            } while ((Double.isNaN(function.fun(performance))) || (Double.isInfinite(function.fun(performance))));

            units[i] = new Unit(performance);


        }


        return units;

    }

    private Unit[] generate(Unit[] parents) {
        Unit[] nextGen = new Unit[size];


        Unit parentA;
        Unit parentB;
        Unit child;

        int parentAIndex = 0;
        int parentBIndex = 0;

        byte[] gene;

        System.arraycopy(parents, 0, nextGen, 0, parents.length);

        for (int i = parents.length; i < size; i++) {


            do {
                //generate child
                do {
                    parentAIndex = (int) (Math.random() * parents.length);
                    parentBIndex = (int) (Math.random() * parents.length);
                }
                while (parentAIndex == parentBIndex);

                gene = new byte[geneTotalLength];

                parentA = parents[parentAIndex];
                parentB = parents[parentBIndex];

                for (int j = 0; j < geneTotalLength; j++) {

                    if (j % 2 == 0)
                        gene[j] = parentA.gene[j];
                    else
                        gene[j] = parentB.gene[j];

                }
                child = new Unit(gene);
                geneMutation(child);
            } while ((Double.isNaN(child.getPhenotype())) || (Double.isInfinite(child.getPhenotype())));

            nextGen[i] = child;


        }

        return nextGen;
    }


    private void geneMutation(Unit unit) {

        for (int i = 0; i < geneLength; i++) {

            if (Math.random() < probabilityOfGeneMutation) {

                if (unit.gene[i] == 0)
                    unit.gene[i] = 1;
                else
                    unit.gene[i] = 0;

            }


        }

    }

    private Unit[] selection(Unit[] lastGen) {


        double phenotypeMax = Arrays.stream(lastGen).mapToDouble(new ToDoubleFunction<Unit>() {
            @Override
            public double applyAsDouble(Unit value) {
                return value.getPhenotype();
            }
        }).max().orElse(0);

        double phenotypeMin = Arrays.stream(lastGen).mapToDouble(new ToDoubleFunction<Unit>() {
            @Override
            public double applyAsDouble(Unit value) {
                return value.getPhenotype();
            }
        }).min().orElse(0);


        //const phenotype
        if (phenotypeMax == phenotypeMin)
            return lastGen;

        LinkedList<Unit> nextGenList = new LinkedList<>();


        for (Unit unit : lastGen) {

            double alive = (unit.getPhenotype() - phenotypeMin) / (phenotypeMax - phenotypeMin);

            switch (selectType) {

                case GETMAX: {
                    if (Math.exp(alive - 1) > Math.random())
                        nextGenList.add(unit);
                    break;
                }
                case GETMIN: {
                    if (Math.exp(alive - 1) < Math.random())
                        nextGenList.add(unit);
                    break;
                }
                default:
                    System.err.println("unknown select type");
                    break;

            }

        }

        return nextGenList.toArray(new Unit[0]);

    }


    public double play(SelectType selectType) {

        this.selectType = selectType;

        LinkedList<Unit> gen = new LinkedList<Unit>(Arrays.asList(generate()));

        units = gen.toArray(new Unit[0]);

        for (int i = 0; i < generationLimits; i++) {
            units = selection(units);
            units = generate(units);
        }

        switch (selectType) {

            case GETMAX: {
                return Arrays.stream(units).max((o1, o2) -> (int) Math.signum(o1.getPhenotype() - o2.getPhenotype())).get().getPhenotype();

            }
            case GETMIN: {
                return Arrays.stream(units).min((o1, o2) -> (int) Math.signum(o1.getPhenotype() - o2.getPhenotype())).get().getPhenotype();
            }
            default:
                System.err.println("unknown select type");
                break;

        }


        return Double.NaN;

    }

    public static void main(String[] args) {

        int x = 8;
        int y = 8;
        int z = 8;

        ValueGenerator valueGenerator = new ValueGenerator();

        List<FunctionMulArgs> functionMulArgs = new LinkedList<>();

        functionMulArgs.add(args1 -> Math.sin(args1[0]) + Math.cos(args1[1]) + 5);
        functionMulArgs.add(args1 -> Math.sin(args1[0]) + Math.cos(args1[1]));
        int[][][] result = valueGenerator.generate(functionMulArgs, x, y, z, -5, 5, -5, 5);


        System.out.println("result size:" + result.length);


    }


}
