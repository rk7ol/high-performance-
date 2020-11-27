#include"test.h"

int main(int argc, char **argv)
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
    //2

    int cube_numb_of_pro ;
    cube_numb_of_pro= temp_index / allrank;

    struct arrcube *myarr = fill_arr_of_cube(x_length, y_length, z_length, myrank, cube_numb_of_pro, temp);
    //2

    //end


    // for(int i=0;i<allrank;i++)
    // {
    //     if(myrank==i)
    //     {
    //         printf("proess:%d\n",myrank);
    //         for(int i=0;i<cube_numb_of_pro;i++)
    //         {
    //             printf("index:%d\n",myarr[i].temp.index);
    //             printf("connect:\n");
    //             for(int j=0;j<6;j++)
    //             {
    //                 printf("%d ",myarr[i].connect[j]);
    //             }
    //             printf("\n");
    //         }
    //     }
    //     sleep(2);
    // }

    //3
 
    MPI_Datatype MPI_send_element;
    commit_new_type(&MPI_send_element);

    process_send_num *con_arr = malloc(allrank * sizeof(process_send_num));

    con_arr = communicator_get_process_send_num_list(myarr, cube_numb_of_pro, allrank);

    struct send_table_element *send_arr = create_send_table(allrank, cube_numb_of_pro, con_arr, myarr);

    fill_send_table(send_arr,myarr,cube_numb_of_pro);
    //3

 
    //4
    communication(allrank, myrank, cube_numb_of_pro, send_arr, con_arr, MPI_send_element);
    //4
    
    MPI_Finalize();
    return 0;
}