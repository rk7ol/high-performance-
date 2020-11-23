#include "avro_serializer.h"

#include "kafka_producer.h"
#include "base64.h"

avro_serializer avro_serializer_create(const char *schemaFilePath, const size_t buffer_size)
{

    avro_schema_t schema;

    /*
        get scheme from file
    */
    FILE *f = fopen(schemaFilePath, "r");
    //get scheme json file length
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *scheme_json;
    scheme_json = malloc(sizeof(char) * size);

    fread(scheme_json, sizeof(char), size, f);

    char scheme_string[size + 1];

    strncpy(scheme_string, scheme_json, size);

    if (avro_schema_from_json_literal(scheme_string, &schema))
    {
        fprintf(stderr, "Unable to parse person schema\n");
        exit(EXIT_FAILURE);
    }

    free(scheme_json);

    /*
        set write buffer
    */
    char *buf = malloc(sizeof(char) * buffer_size);

    FILE *tmp = fmemopen(buf, (sizeof(char) * buffer_size), "wb");

    avro_file_writer_t writer;

    if (avro_file_writer_create_fp(tmp, "", 0, schema, &writer))
    {
        fprintf(stderr, "There was an error creating %s\n", "file in memory");
        fprintf(stderr, " error message: %s\n", avro_strerror());
        exit(EXIT_FAILURE);
    }

    avro_serializer serializer = {
        .buf = buf,
        .tmp = tmp,
        .schema = schema,
        .value_class = avro_generic_class_from_schema(schema),
        .writer = writer,
        .buffer_size = buffer_size};

    return serializer;
}

void avro_serializer_serialize(const avro_serializer *serializer, const avro_value_t *record)
{

    if (avro_file_writer_append_value(serializer->writer, (avro_value_t *)record))
    {
        fprintf(stderr,
                "Unable to write Person value to memory buffer\nMessage: %s\n", avro_strerror());
        exit(EXIT_FAILURE);
    }
}

void avro_serializer_writer_flush(avro_serializer *serializer)
{
    avro_file_writer_flush(serializer->writer);
}

avro_value_t avro_serializer_createRecord(avro_serializer *serializer)
{

    avro_value_t record;
    avro_generic_value_new(serializer->value_class, &record);

    return record;
}

void avro_record_set_int(avro_value_t *record, const char *key, const int value)
{
    avro_value_t avro_value;

    if (avro_value_get_by_name(record, key, &avro_value, NULL) == 0)
    {
        avro_value_set_int(&avro_value, value);
    }
}

void avro_record_set_long(avro_value_t *record, const char *key, const long value)
{
    avro_value_t avro_value;

    if (avro_value_get_by_name(record, key, &avro_value, NULL) == 0)
    {
        avro_value_set_long(&avro_value, value);
    }
}

void avro_record_set_double(avro_value_t *record, const char *key, const double value)
{
    avro_value_t avro_value;

    if (avro_value_get_by_name(record, key, &avro_value, NULL) == 0)
    {
        avro_value_set_double(&avro_value, value);
    }
}

void avro_record_set_string(avro_value_t *record, const char *key, const char *value)
{
    avro_value_t avro_value;

    if (avro_value_get_by_name(record, key, &avro_value, NULL) == 0)
    {
        avro_value_set_string(&avro_value, value);
    }
}

void avro_serializer_destroy(avro_serializer *serializer)
{

    avro_value_iface_decref(serializer->value_class);
    avro_schema_decref(serializer->schema);

    //avro_writer_free(serializer->writer);
    avro_file_writer_flush(serializer->writer);
    avro_file_writer_close(serializer->writer);
    free(serializer->buf);
}
