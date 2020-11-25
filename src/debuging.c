//热传导之通信部分
#include<stdio.h>
#include"mpi.h"
#include<stdlib.h>
#define x_length 2
#define y_length 2
#define z_length 2

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
    int pro_t;//进程号
    int element_numb;//需要向该进程发送的元素个数
    int current_element_numb;
    struct send_element * element;
};

struct process_send_num
{
    int process_num;
    int send_num;
};

typedef struct process_send_num process_send_num;

int communicator_get_process_num_by_index(int index,int cube_numb)
{   
    int i;
    i=index/cube_numb;
    return i;
};



process_send_num *communicator_get_process_send_num_list(const struct arrcube *arr_cube, const size_t size, const size_t process_size)
{
        //initialize
    process_send_num *result = malloc(sizeof(process_send_num) * process_size);

    for (size_t i = 0; i < process_size; i++)
    {
        result[i].process_num = i;

        for (size_t j = 0; j < 6; j++)
        {
            result[i].send_num = 0;
        }
    }

    //add send num
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {

            if (arr_cube[i].connect[j] != -1)
            {
                //find neighber block index
                int proces_num = communicator_get_process_num_by_index(arr_cube[i].connect[j],size);
                result[proces_num].send_num++;
            }
        }
    }

    return result;
};

int main(int argc,char**argv)
{
    int myrank;//进程编号标识符
    int allrank;//总进程数

//section1 //初始化方块矩阵，并编号，方块编号从1号开始
    int arr[x_length][y_length][z_length];
    for(int i=0;i<x_length;i++)
    {
        for(int j=0;j<y_length;j++)
        {
            for(int k=0;k<z_length;k++)
            {   
                arr[i][j][k]=1;
            }
        }
    }
    struct cube temp[x_length][y_length][z_length];
    int temp_index=1;
    for(int i=0;i<x_length;i++)
    {
        for(int j=0;j<y_length;j++)
        {
            for(int k=0;k<z_length;k++)
            {   
                //假如是内部方块
                if(arr[i][j][k]==1)
                {
                    temp[i][j][k].x=i;
                    temp[i][j][k].y=j;
                    temp[i][j][k].z=k;
                    temp[i][j][k].index=temp_index;
                    temp[i][j][k].tempa=temp_index;
                    ++temp_index;
                }
                //外部方块（环境方块）
                else
                {
                    temp[i][j][k].x=-1;
                    temp[i][j][k].y=-1;
                    temp[i][j][k].z=-1;
                    temp[i][j][k].index=-1;
                    temp[i][j][k].tempa=-0.1;
                }
                //printf("temp:x%d,\ntemp:y%d,\ntemp:z%d,\ntemp:index%d,\ntemp:tempa%f,\n",temp[i][j][k].x,temp[i][j][k].y,temp[i][j][k].z,temp[i][j][k].index,temp[i][j][k].tempa);
            }
        }
    }
//section1 end
    //printf("section 1 finshed\n");

    //开始并行
    MPI_Status status;   
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&allrank);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

//section2 将三位方块数组拉成二维。方块数量为temp_index-1
    //遍历三位数组
    int cube_numb_of_pro=(temp_index-1)/allrank;//进程负责的方块数量
    struct arrcube *myarr=malloc(sizeof(struct arrcube)*cube_numb_of_pro);
    for(int i=0;i<x_length;i++)
    {
        for(int j=0;j<y_length;j++)
        {
            for(int k=0;k<z_length;k++)
            {   
                //填充该进程负责的myarr中的数据
                //假如某个方块属于该进程负责
                if((temp[i][j][k].index>myrank*cube_numb_of_pro)&&(temp[i][j][k].index<=(myrank*cube_numb_of_pro+cube_numb_of_pro)))
                {
                    int myarr_index;
                    if(temp[i][j][k].index==myrank*cube_numb_of_pro+cube_numb_of_pro)
                    {
                        myarr_index=temp[i][j][k].index%cube_numb_of_pro+cube_numb_of_pro-1;
                    }   
                    else
                    {
                        myarr_index=temp[i][j][k].index%cube_numb_of_pro-1;
                    }

                    //将该方块放置其对应位置
                    myarr[myarr_index].temp=temp[i][j][k];

                    /*填充方块的相邻关系index，若某方块的index为-1，则该方块为外部方块*/                    
                    myarr[myarr_index].connect[0]=temp[i+1][j][k].index;
                    myarr[myarr_index].connect[1]=temp[i-1][j][k].index;
                    myarr[myarr_index].connect[2]=temp[i][j+1][k].index;
                    myarr[myarr_index].connect[3]=temp[i][j-1][k].index;
                    myarr[myarr_index].connect[4]=temp[i][j][k+1].index;
                    myarr[myarr_index].connect[5]=temp[i][j][k-1].index;
                    if(i==x_length-1)
                    {
                        myarr[myarr_index].connect[0]=-1;
                    }
                    if(j==y_length-1)
                    {
                        myarr[myarr_index].connect[2]=-1;
                    }
                    if(k==z_length-1)
                    {
                        myarr[myarr_index].connect[4]=-1;
                    }
                    if(i==0)
                    {
                        myarr[myarr_index].connect[1]=-1;
                    }
                    if(j==0)
                    {
                        myarr[myarr_index].connect[3]=-1;
                    }
                    if(k==0)
                    {
                        myarr[myarr_index].connect[5]=-1;
                    }
                }
            }
        }
    }
//section2 end

    if(myrank==0)
    {
        printf("proess:%d\n",myrank);
        for(int i=0;i<cube_numb_of_pro;i++)
        {
            printf("index:%d\n",myarr[i].temp.index);
            printf("connect:\n");
            for(int j=0;j<6;j++)
            {
                printf("%d ",myarr[i].connect[j]);
            }
            printf("\n");
        }
        sleep(2);
    }
    else
    {
                printf("proess:%d\n",myrank);
        for(int i=0;i<cube_numb_of_pro;i++)
        {
            printf("index:%d\n",myarr[i].temp.index);
            printf("connect:\n");
            for(int j=0;j<6;j++)
            {
                printf("%d ",myarr[i].connect[j]);
            }
            printf("\n");
        }
    }
    

//section3 通信前的准备工作 


    //建立新的Datatype
    struct send_element send_temp;
    MPI_Datatype MPI_send_element;
    MPI_Aint disp[2];
    int block[2]={1,1};
    MPI_Datatype type[2]={MPI_INT,MPI_DOUBLE};
    MPI_Get_address(&send_temp.index,&disp[0]);
    MPI_Get_address(&send_temp.tempa,&disp[1]);
    disp[1]=disp[1]-disp[0];
    disp[0]=0;
    MPI_Type_create_struct(2,&block,&disp,&type,&MPI_send_element);
    MPI_Type_commit(&MPI_send_element);
    //建立完毕

    //新建辅助数组，存放的内容为与第n号进程相邻的方块数目
    process_send_num *con_arr=malloc(allrank*sizeof(process_send_num));    
        //遍历各自的index_temp散列表，填充con_arr;
    printf("1\n\n\n\n");
    con_arr=communicator_get_process_send_num_list(&myarr,cube_numb_of_pro,allrank);
    //建立且填充完毕
    printf("2\n\n\n\n");

    MPI_Finalize();
    return 0;
}