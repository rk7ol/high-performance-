#include "kafka_producer.h"
#include "base64.h"
#include <string.h>

char errstr[512]; /* librdkafka API error reporting buffer */

kafka_producer kafka_producer_initilize(const char *brokers)
{

    rd_kafka_conf_t *conf = rd_kafka_conf_new();
    if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers,
                          errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
    {
        fprintf(stderr, "%s\n", errstr);
        exit(EXIT_FAILURE);
    }

    rd_kafka_t *rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));

    if (!rk)
    {
        fprintf(stderr,
                "%% Failed to create new producer: %s\n", errstr);
        exit(EXIT_FAILURE);
    }

    kafka_producer producer = {
        .rk = rk,
        .conf = conf};

    return producer;
}

void kafka_producer_produce(kafka_producer *producer, const char *message, size_t len, const char *topic)
{

    rd_kafka_resp_err_t err = rd_kafka_producev(
        /* Producer handle */
        producer->rk,
        /* Topic name */
        RD_KAFKA_V_TOPIC(topic),
        /* Make a copy of the payload. */
        RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
        /* Message value and length */
        RD_KAFKA_V_VALUE((void *)message, len),
        /* Per-Message opaque, provided in
         * delivery report callback as
         * msg_opaque. */
        RD_KAFKA_V_OPAQUE(NULL),
        /* End sentinel */
        RD_KAFKA_V_END);

    if (err)
    {
        /*
                         * Failed to *enqueue* message for producing.
                         */
        fprintf(stderr,
                "%% Failed to produce to topic %s: %s\n",
                topic, rd_kafka_err2str(err));

        if (err == RD_KAFKA_RESP_ERR__QUEUE_FULL)
        {
            /* If the internal queue is full, wait for
             * messages to be delivered and then retry.
             * The internal queue represents both
             * messages to be sent and messages that have
             * been sent or failed, awaiting their
             * delivery report callback to be called.
             *
             * The internal queue is limited by the
             * configuration property
             * queue.buffering.max.messages */
            rd_kafka_poll(producer->rk, 1000 /*block for max 1000ms*/);
        }
    }
    else
    {
        fprintf(stderr, "%% Enqueued message (%zd bytes) "
                        "for topic %s\n",
                len, topic);
    }

    /* A producer application should continually serve
        * the delivery report queue by calling rd_kafka_poll()
        * at frequent intervals.
        * Either put the poll call in your main loop, or in a
        * dedicated thread, or call it after every
        * rd_kafka_produce() call.
        * Just make sure that rd_kafka_poll() is still called
        * during periods where you are not producing any messages
        * to make sure previously produced messages have their
        * delivery report callback served (and any other callbacks
        * you register). */
    rd_kafka_poll(producer->rk, 0 /*non-blocking*/);
}

void kafka_producer_produce_file(kafka_producer *producer, FILE *fp, const char *content, const char *topic)
{

    fseek(fp, 0, SEEK_END);

    const char *result = malloc(Base64encode_len(ftell(fp)));
    Base64encode((char *)result, content, ftell(fp));

    kafka_producer_produce(producer, (void *)result, strlen(result), topic);
}

void kafka_producer_destroy(kafka_producer *producer)
{

    /* Wait for final messages to be delivered or fail.
         * rd_kafka_flush() is an abstraction over rd_kafka_poll() which
         * waits for all messages to be delivered. */
    fprintf(stderr, "%% Flushing final messages..\n");
    rd_kafka_flush(producer->rk, 10 * 1000 /* wait for max 10 seconds */);

    /* If the output queue is still not empty there is an issue
         * with producing messages to the clusters. */
    if (rd_kafka_outq_len(producer->rk) > 0)
        fprintf(stderr, "%% %d message(s) were not delivered\n",
                rd_kafka_outq_len(producer->rk));

    /* Destroy the producer instance */
    rd_kafka_destroy(producer->rk);
}
