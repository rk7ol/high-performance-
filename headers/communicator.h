

#ifndef _COMMUNICATOR_H_
#define _COMMUNICATOR_H_

#include <stdlib.h>


#define x_length 50
#define y_length 50
#define z_length 50


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

struct send_element
{
    int index;
    double tempa;
};

typedef struct send_element send_element;

struct send_table_element
{
    int pro_t;
    int element_numb;
    int current_element_numb;
    struct send_element * element;
};

typedef struct send_table_element send_table_element;


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



/**
 * @brief  filling the arrcube
 * 
 * @param 三维数组
 * @param myrank
 * @param  cube_numb_of_pro
 * @return arr_cube of each process
 */
extern  arr_cube * fillarrcube(/*三维数组*/int myrank,int cube_numb_of_pro); 

/**
 * @brief create send table
 * 
 * @param allrank
 * @return send_table
 */
extern send_table_element* create_send_table(int allrank);

/**
 * @brief filling the send table
 * 
 * @param cube_numb_of_pro
 * @return send_table
 */
extern send_table_element* fill_send_table(int cube_numb_of_pro);



#endif