

#ifndef _COMMUNICATOR_H_
#define _COMMUNICATOR_H_

#include <stdlib.h>
#include "cHashMap.h"
#include"mpi.h"
#include <stdio.h>

int x_length,y_length,z_length;
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
    struct send_element *element;
};

typedef struct send_table_element send_table_element;

/**
 * @brief get process num by cube index
 * 
 * @param index cube index
 * @param process_num 
 * @return int process num
 */
extern int communicator_get_process_num_by_index(int index,int cube_numb);

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
 * @brief get max temperature, use reduce and boradcast
 * 
 * @param temp send buffer, delta of each node
 * @param root 
 */
extern void communicator_get_max_temp_delta(double *max_delta, double *temp, MPI_Comm comm, int root);







typedef struct process_send_num process_send_num;




//创建三维数组
extern int ***create_three_dimen_arr(int m,int n,int t);

//填充01三维数组
extern int *** fill_01_arr(char * pathname);

//创建三维结构体数组
extern struct cube*** create_three_dimen_struct_cube(int m,int n,int t);

//填充三维结构体数组
extern int fill_arr(struct cube *** temp,int ***arr);

//填充二维数组
extern struct arr_cube *fill_arr_of_cube(int x,int y,int z,int myrank,int cube_numb_of_pro,struct cube *** temp);

//新建发送表
extern struct send_table_element * create_send_table(int allrank,int cube_numb_of_pro,process_send_num * con_arr,struct arr_cube * myarr);

//填充发送表
extern void fill_send_table(struct send_table_element * send_arr, struct arr_cube *myarr,int cube_numb_of_pro,int allrank);

//新建MPI_Datatype
extern void commit_new_type(MPI_Datatype *MPI_send_element);

//通信
extern void communication(int allrank,int myrank,int cube_numb_of_pro,struct send_table_element *send_arr,process_send_num *con_arr,MPI_Datatype MPI_send_element, HashMap* index_tem_table);

#endif