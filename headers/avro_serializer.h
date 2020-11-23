#ifndef _AVRO_SERIALIZER_H_
#define _AVRO_SERIALIZER_H_

#include <avro.h>
#include <stdlib.h>

/**
 * @brief avro serializer
 *        serializer data
 * 
 */
struct avro_serializer
{
    char *buf;
    FILE *tmp;
    const size_t buffer_size;
    avro_file_writer_t writer;
    avro_schema_t schema;
    avro_value_iface_t *value_class;
};

typedef struct avro_serializer avro_serializer;

/**
 * @brief create avro serializer
 * 
 * @param schemaFilePath avsc file path
 * @param buffer_size serializer buffer size
 * @return avro_serializer 
 */
extern avro_serializer avro_serializer_create(const char *schemaFilePath, const size_t buffer_size);

/**
 * @brief serialize record
 * 
 * @param serializer 
 * @param record 
 */
extern void avro_serializer_serialize(const avro_serializer *serializer, const avro_value_t *record);

/**
 * @brief reset serializer buffer to zero array
 * 
 * @param serializer 
 */
extern void avro_serializer_writer_flush(avro_serializer *serializer);

/**
 * @brief create a record
 * 
 * @param serializer 
 * @return avro_value_t 
 */
extern avro_value_t avro_serializer_createRecord(avro_serializer *serializer);

/**
 * @brief set int value by key
 * 
 * @param record 
 * @param key 
 * @param value 
 */
extern void avro_record_set_int(avro_value_t *record, const char *key, const int value);

/**
 * @brief set long value by key
 * 
 * @param record 
 * @param key 
 * @param value 
 */
extern void avro_record_set_long(avro_value_t *record, const char *key, const long value);

/**
 * @brief set double value by key
 * 
 * @param record 
 * @param key 
 * @param value 
 */
extern void avro_record_set_double(avro_value_t *record, const char *key, const double value);

/**
 * @brief set string value by key
 * 
 * @param record 
 * @param key 
 * @param value 
 */
extern void avro_record_set_string(avro_value_t *record, const char *key, const char *value);

/**
 * @brief free avro serializer
 * 
 * @param serializer 
 */
extern void avro_serializer_destroy(avro_serializer *serializer);

#endif