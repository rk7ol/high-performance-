package HighPerformanceHC.communicator;

import java.util.List;

public interface Serializer<E> {

    byte[] serialize(List<E> dataList);

}
