#include "communicator.h"

// process_send_num *communicator_get_process_send_num_list(const arr_cube *arr_cube, const size_t size, const size_t process_size)
// {

//     //initialize
//     process_send_num *result = malloc(sizeof(process_send_num) * process_size);

//     for (size_t i = 0; i < process_size; i++)
//     {
//         result[i].process_num = i;

//         for (size_t j = 0; j < 6; j++)
//         {
//             result[i].send_num = 0;
//         }
//     }

//     //add send num
//     for (size_t i = 0; i < size; i++)
//     {
//         for (size_t j = 0; j < 6; j++)
//         {

//             if (arr_cube[i].connect[j] != -1)
//             {
//                 //find neighber block index
//                 int proces_num = communicator_get_process_num_by_index(arr_cube[i].connect[j]);
//                 result[proces_num].send_num++;
//             }
//         }
//     }

//     return result;
// }