package native_implement;

public class Invoker {

    static {
        System.loadLibrary("HP");
    }


    public native void InvokeMPIHeatConduct(String targetPath, int[][][] calculateValue);
}
