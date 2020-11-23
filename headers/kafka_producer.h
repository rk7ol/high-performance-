

#ifndef _KAFKA_PRODUCER_H_
#define _KAFKA_PRODUCER_H_

#include <librdkafka/rdkafka.h>
#include <stdlib.h>

/**
 * @brief kafka produer wrapper
 * 
 */
struct kafka_producer
{
    rd_kafka_t *rk;        /* Producer instance handle */
    rd_kafka_conf_t *conf; /* Temporary configuration object */
};

typedef struct kafka_producer kafka_producer;

/**
 * @brief initilize a kafka producer
 * 
 * @param brokers 
 * @return kafka_producer 
 */
extern kafka_producer kafka_producer_initilize(const char *brokers);

/**
 * @brief produce a message to topic
 * 
 * @param producer 
 * @param message 
 * @param topic 
 */
extern void kafka_producer_produce(kafka_producer *producer, const char *message, size_t len, const char *topic);

/**
 * @brief produce content in file, content encode by base64 mime
 * 
 * @param producer 
 * @param fp 
 * @param content file content
 * @param topic 
 */
void kafka_producer_produce_file(kafka_producer *producer, FILE *fp, const char *content, const char *topic);

/**
 * @brief destroy kafka producer
 * 
 * @param producer 
 */
extern void kafka_producer_destroy(kafka_producer *producer);

#endif