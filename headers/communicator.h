

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
 * @brief get process num by cube index
 * 
 * @param index cube index
 * @param process_num 
 * @return int process num
 */
extern int communicator_get_process_num_by_index(int index);

/**
 * @brief 
 * 
 * @param arr_cube 
 * @param size 
 * @param process_size 
 * @return process_send_num*  (process num)<->(send num), size : process size
 */
extern process_send_num *communicator_get_process_send_num_list(const arr_cube *arr_cube, const size_t size, const size_t process_size);

#endif