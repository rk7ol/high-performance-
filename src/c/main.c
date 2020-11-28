#include <stdio.h>
#include <string.h>

#include "communicator.h"
#include "heat_conduct.h"
#include "kafka_producer.h"
#include "avro_serializer.h"

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
    int myrank;  //进程编号标识符
    int allrank; //总进程数
    int ***arr = create_three_dimen_arr(x_length, y_length, z_length);
    for (int i = 0; i < x_length; i++)
    {
        for (int j = 0; j < y_length; j++)
        {
            for (int k = 0; k < z_length; k++)
            {
                arr[i][j][k] = 1;
            }
        }
    }

    struct cube ***temp = create_three_dimen_struct_cube(x_length, y_length, z_length);

    int temp_index = fill_arr(temp, arr);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &allrank);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    int cube_numb_of_pro;
    cube_numb_of_pro = temp_index / allrank;

    arr_cube *myarr = fill_arr_of_cube(x_length, y_length, z_length, myrank, cube_numb_of_pro, temp);

    MPI_Datatype MPI_send_element;
    commit_new_type(&MPI_send_element);

    process_send_num *con_arr = malloc(allrank * sizeof(process_send_num));

    con_arr = communicator_get_process_send_num_list(myarr, cube_numb_of_pro, allrank);

    struct send_table_element *send_arr = create_send_table(allrank, cube_numb_of_pro, con_arr, myarr);

    double *delta = malloc(sizeof(double));
    double *max_delta = malloc(sizeof(double));

    HashMap index_temp_table = heat_conduct_create_index_temp_table(myarr, cube_numb_of_pro);


    int count = 0;

    do
    {

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