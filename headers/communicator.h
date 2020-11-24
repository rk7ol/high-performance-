

#ifndef _COMMUNICATOR_H_
#define _COMMUNICATOR_H_

#include <stdlib.h>

struct cube
{
    int x;
    int y;
    int z;
    int index;
    double tempa;
};

typedef struct cube cube;

struct arr_cube
{
    cube temp;
    int connect[6];
};

typedef struct arr_cube arr_cube;

struct process_send_num
{
    int process_num;
    int send_num;
};

typedef struct process_send_num process_send_num;

/**
 * @brief  
 * 
 * @param arr_cube 
 * @param size 
 * @return process_send_num* 
 */
extern process_send_num *communicator_get_process_send_num_list(const arr_cube *arr_cube, const size_t size);

#endif