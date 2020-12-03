package HighPerformanceHC.communicator;

import org.apache.avro.Schema;
import org.apache.avro.file.DataFileReader;
import org.apache.avro.file.SeekableByteArrayInput;
import org.apache.avro.generic.GenericDatumReader;
import org.apache.avro.generic.GenericRecord;
import org.apache.avro.io.DatumReader;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;

public class AvroDeserializer implements Deserializer<GenericRecord> {

    //deserialize tools
    private final DatumReader<GenericRecord> reader;
    private DataFileReader<GenericRecord> genericDataReader;
    private SeekableByteArrayInput byteArrayInput;


    public AvroDeserializer(Schema schema) {

        //deserialize tools init
        reader = new GenericDatumReader<>(schema);

    }

    public List<GenericRecord> deserialize(byte[] dataByteArray) {


        List<GenericRecord> recordList = new LinkedList<>();

        byteArrayInput = new SeekableByteArrayInput(dataByteArray);

        try {
            genericDataReader = new DataFileReader<>(byteArrayInput, reader);

            while (genericDataReader.hasNext())
                recordList.add(genericDataReader.next());

            genericDataReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {

            System.err.println("illegal");

        }

        return recordList;
    }


}
