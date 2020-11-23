#include <stdio.h>
#include <string.h>

#include "cHashMap.h"
#include "cArrayList.h"
#include "util.h"
#include "avro_serializer.h"
#include "kafka_producer.h"

int cube_temp_hash(const void *content)
{

    cube_temp *c = (cube_temp *)content;

    return c->index;
}

void arrayList_example()
{

    //create
    ArrayList arrayList = ArrayList_create(sizeof(int));

    //add
    for (size_t i = 0; i < 100; i++)
    {
        ArrayList_add(&arrayList, &i);
    }

    //get and print
    for (size_t i = 0; i < arrayList.current_size; i++)
    {
        printf("value: %d\n", *(int *)ArrayList_get(&arrayList, i));
    }

    //free
    ArrayList_destroy(&arrayList);
}

void hashMap_example()
{

    //while (1)
    //{
    //create
    HashMap hashMap = HashMap_create(&cube_temp_hash, sizeof(int), 0.75);

    //put
    for (size_t i = 0; i < 10000; i++)
    {
        cube_temp *c1 = malloc(sizeof(cube_temp));

        c1->index = i;
        c1->temp = i + 1.0;

        HashMap_put(&hashMap, c1, c1);
    }

    //replace
    cube_temp *c2 = malloc(sizeof(cube_temp));

    c2->index = 40;
    c2->temp = 8000.888;

    HashMap_repalce(&hashMap, c2, c2);

    //get
    for (size_t i = 0; i < 100; i++)
    {
        cube_temp c1 = {
            .index = i,
            .temp = i + 1.0};
        cube_temp *cube = (cube_temp *)HashMap_get(&hashMap, &c1);

        printf("out value:%d, %lf\n", cube->index, cube->temp);
    }

    HashMap_destroy(&hashMap);
    //}
}

/*
*   send 100 point
*
*/

void send_message_example(char *schemaFilePath)
{

    avro_serializer serializer = avro_serializer_create(schemaFilePath, 4096);

    kafka_producer producer = kafka_producer_initilize("dodlee.cn:9092");

    for (int i = 0; i < 100; i++)
    {
        avro_value_t record = avro_serializer_createRecord(&serializer);
        avro_record_set_int(&record, "coord_x", i);
        avro_record_set_int(&record, "coord_y", 2 * i);
        avro_record_set_int(&record, "coord_z", 3 * i);
        avro_record_set_double(&record, "temp", (double)i * 0.5);
        avro_serializer_serialize(&serializer, &record);
        avro_serializer_writer_flush(&serializer);
    }

    kafka_producer_produce_file(&producer, serializer.tmp, serializer.buf, "test");

    kafka_producer_destroy(&producer);
    avro_serializer_destroy(&serializer);
}

int main(int argc, char *argv[])
{
    send_message_example(argv[1]);

    //hashMap_example();

    //arrayList_example();

    return 0;
}