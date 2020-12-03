package HighPerformanceHC.communicator;

import org.apache.avro.generic.GenericRecord;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.common.serialization.StringDeserializer;

import java.time.Duration;
import java.util.Base64;
import java.util.LinkedList;
import java.util.List;
import java.util.Properties;

public class DataConsumer {
    private final KafkaConsumer<String, String> consumer;
    private final AvroDeserializer deserializer;
    private List<String> topicList;

    public DataConsumer(AvroDeserializer deserializer) {

        this.deserializer = deserializer;

        Properties props = new Properties();
        props.setProperty("bootstrap.servers", "dodlee.cn:9092");
        props.setProperty("group.id", "1");
        props.setProperty("enable.auto.commit", "false");
        props.setProperty("auto.commit.interval.ms", "100");
        //props.put("max.partition.fetch.bytes", "140");
        consumer = new KafkaConsumer<>(props, new StringDeserializer(), new StringDeserializer());

    }


    public void subscribe(List<String> topicList) {

        this.topicList = topicList;
        consumer.subscribe(topicList);

    }


    public List<GenericRecord> receive() {

        List<GenericRecord> receiveList = new LinkedList<>();

        ConsumerRecords<String, String> records = consumer.poll(Duration.ofMillis(100));


        for (ConsumerRecord<String, String> record : records) {
            System.out.println("receive size:" + record.value().getBytes().length);

            //decode
            receiveList.addAll(deserializer.deserialize(decode(record.value().getBytes())));

            consumer.commitAsync();
        }

        return receiveList;
    }


    private byte[] decode(byte[] bytes) {

        return Base64.getMimeDecoder().decode(bytes);
    }


}
