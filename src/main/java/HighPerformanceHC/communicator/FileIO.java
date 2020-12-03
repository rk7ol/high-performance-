package HighPerformanceHC.communicator;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.List;

public class FileIO {

    public interface Writable {


        String getStringSequence();


    }


    public void write(String path, List<? extends Writable> writableList) {

        File file = new File(path);

        try {
            FileWriter writer = new FileWriter(file, true);


            for (Writable writable : writableList) {

                writer.write(writable.getStringSequence());
                writer.write("\n");

            }


            writer.flush();
            writer.close();


        } catch (IOException e) {
            e.printStackTrace();
        }


    }

}
