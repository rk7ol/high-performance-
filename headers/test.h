#include "mpi.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#ifndef _TEST_H  
#define _TEST_H

#define x_length 16
#define y_length 16
#define z_length 16



struct cube
{
    int x;
    int y;
    int z;
    int index;
    double tempa;
};

struct arrcube
{
    struct cube temp;
    int connect[6];
};

struct send_element
{
    int index;
    double tempa;
};

struct send_table_element
{
    int pro_t;        //进程号
    int element_numb; //需要向该进程发送的元素个数
    int current_element_numb;
    struct send_element *element;
};

struct process_send_num
{
    int process_num;
    int send_num;
};

typedef struct process_send_num process_send_num;

//通过index获得进程号
extern int communicator_get_process_num_by_index(int index, int cube_numb);

//返回相邻方块数目
extern process_send_num *communicator_get_process_send_num_list(const struct arrcube *arr_cube, const size_t size, const size_t process_size);

//创建三维数组
extern int ***create_three_dimen_arr(int m,int n,int t);

//创建三维结构体数组
extern struct cube*** create_three_dimen_struct_cube(int m,int n,int t);

//填充三维结构体数组
extern int fill_arr(struct cube *** temp,int ***arr);

//填充二维数组
extern struct arrcube *fill_arr_of_cube(int x,int y,int z,int myrank,int cube_numb_of_pro,struct cube *** temp);

//新建发送表
extern struct send_table_element * create_send_table(int allrank,int cube_numb_of_pro,process_send_num * con_arr,struct arrcube * myarr);

//填充发送表
extern void fill_send_table(struct send_table_element * send_arr, struct arrcube *myarr,int cube_numb_of_pro,int allrank);

//新建MPI_Datatype
extern void commit_new_type(MPI_Datatype *MPI_send_element);

//通信
extern void communication(int allrank,int myrank,int cube_numb_of_pro,struct send_table_element *send_arr,process_send_num *con_arr,MPI_Datatype MPI_send_element);
#endif //_TEST_H