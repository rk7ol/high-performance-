package HighPerformanceHC.communicator;

import java.util.List;

public interface Deserializer<E> {

    List<E> deserialize(byte[] dataByteArray);
}
