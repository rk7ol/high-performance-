#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "communicator.h"
#include "heat_conduct.h"
#include "kafka_producer.h"
#include "avro_serializer.h"
#include "utils.h"

/*
*   send 100 point
*
*/

void send_message_example(char *schemaFilePath, arr_cube *send_buffer, int send_number, long round)
{

    avro_serializer serializer = avro_serializer_create(schemaFilePath, 819600);

    kafka_producer producer = kafka_producer_initilize("dodlee.cn:9092");

    for (int i = 0; i < send_number; i++)
    {
        avro_value_t record = avro_serializer_createRecord(&serializer);

        avro_record_set_long(&record, "round", round);
        avro_record_set_int(&record, "coord_x", send_buffer[i].temp.x);
        avro_record_set_int(&record, "coord_y", send_buffer[i].temp.y);
        avro_record_set_int(&record, "coord_z", send_buffer[i].temp.z);
        avro_record_set_double(&record, "temp", send_buffer[i].temp.tempa);
        avro_serializer_serialize(&serializer, &record);
        avro_serializer_writer_flush(&serializer);
    }

    kafka_producer_produce_file(&producer, serializer.tmp, serializer.buf, "test");

    kafka_producer_destroy(&producer);
    avro_serializer_destroy(&serializer);
}

int main(int argc, char *argv[])
{

    char *fifo_name = argv[1];
    int fid;

    x_length = string2int(argv[2]);

    y_length = string2int(argv[3]);

    z_length = string2int(argv[4]);

    MPI_Status status;
    int myrank;  //进程编号标识符
    int allrank; //总进程数

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &allrank);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    int result[x_length][y_length][z_length];

    if (myrank == 0)
    {

        fid = open(fifo_name, O_RDONLY);

        read(fid, &result, sizeof(int) * x_length * y_length * z_length);
        close(fid);
    }

    // for (size_t i = 0; i < x_length; i++)
    // {

    //     for (size_t j = 0; j < y_length; j++)
    //     {
    //         for (size_t k = 0; k < z_length; k++)
    //         {
    //             result[i][j][k] = 1;
    //         }
    //     }
    // }

    MPI_Bcast(&result, x_length * y_length * z_length, MPI_INT, 0, MPI_COMM_WORLD);

    int *(**pointer) = malloc(sizeof(int **) * x_length);

    for (size_t i = 0; i < x_length; i++)
    {
        pointer[i] = malloc(sizeof(int *) * y_length);
        for (size_t j = 0; j < y_length; j++)
        {
            pointer[i][j] = malloc(sizeof(int) * z_length);
            for (size_t k = 0; k < z_length; k++)
            {
                pointer[i][j][k] = result[i][j][k];
            }
        }
    }

    struct cube ***temp = create_three_dimen_struct_cube(x_length, y_length, z_length);
    int temp_index = fill_arr(temp, pointer);

    int cube_numb_of_pro;

    cube_numb_of_pro = temp_index / allrank;
    printf("index numb:%d\n",temp_index);
    //return 0;
    arr_cube *myarr = fill_arr_of_cube(x_length, y_length, z_length, myrank, cube_numb_of_pro, temp);

    MPI_Datatype MPI_send_element;
    commit_new_type(&MPI_send_element);

    process_send_num *con_arr = malloc(allrank * sizeof(process_send_num));

    con_arr = communicator_get_process_send_num_list(myarr, cube_numb_of_pro, allrank);

    struct send_table_element *send_arr = create_send_table(allrank, cube_numb_of_pro, con_arr, myarr);

    double *delta = malloc(sizeof(double));
    double *max_delta = malloc(sizeof(double));

    HashMap index_temp_table = heat_conduct_create_index_temp_table(myarr, cube_numb_of_pro);

    long count = 0;

    do
    {

        send_message_example("resources/Point.avsc", myarr, cube_numb_of_pro, count);

        MPI_Barrier(MPI_COMM_WORLD);
        count++;
        sleep(1);

        *delta = heat_conduct_play(myarr, cube_numb_of_pro, &index_temp_table, myrank);

        fill_send_table(send_arr, myarr, cube_numb_of_pro, allrank);
        communication(allrank, myrank, cube_numb_of_pro, send_arr, con_arr, MPI_send_element, &index_temp_table);

        communicator_get_max_temp_delta(max_delta, delta, MPI_COMM_WORLD, 0);

        if (myrank == 0)
        {
            printf("delta: %f\n", *max_delta);
        }

        //} while (!heat_conduct_isDone(*max_delta) && (count++ < 100));

    } while (!heat_conduct_isDone(*max_delta));

    printf("done\n");

    for (size_t i = 0; i < cube_numb_of_pro; i++)
    {
        printf("rank[%d], temp:<%f>\n", myrank, myarr[i].temp.tempa);
    }

    MPI_Finalize();

    return 0;
}