#include "communicator.h"

int *** fill_01_arr(char * pathname)
{
    int ***a;
   char * filepath="/home/dx123/data_fixed02";
    FILE* fp = fopen(filepath, "r");
    if (!fp) return 0;
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    int cube_size=(size/4-3)/3;
    FILE* input = fopen(filepath, "rb");
    if (input == NULL) {
        printf("无法打开文件");
        exit(0);
    }
    fread(&x_length, sizeof(int), 1/*读取1个数据项*/, input);
    fread(&y_length, sizeof(int), 1/*读取1个数据项*/, input);
    fread(&z_length, sizeof(int), 1/*读取1个数据项*/, input);
    x_length=x_length>> 24;
    y_length=y_length>> 24;
    z_length=z_length>> 24;

    a=create_three_dimen_arr(x_length,y_length,z_length);
    for (int i = 0; i < x_length; i++)
        for (int j = 0; j < y_length; j++)
            for (int k = 0; k < z_length; k++)
                {
                    *(*(*(a + i) + j) + k) = 0;
                }

                
    int temp_x, temp_y, temp_z;//临时坐标
    for (int i = 0; i < cube_size; i++)
    {
        fread(&temp_x, sizeof(int), 1, input);
        fread(&temp_y, sizeof(int), 1, input);
        fread(&temp_z, sizeof(int), 1, input);
        temp_x = temp_x >> 24;
        temp_y = temp_y >> 24;
        temp_z = temp_z >> 24;
        a[temp_x-1][temp_y-1][temp_z-1] = 1;

    }
    return a;
}





process_send_num *communicator_get_process_send_num_list(const arr_cube *arr_cube, const size_t size, const size_t process_size)
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
                int proces_num = communicator_get_process_num_by_index(arr_cube[i].connect[j], size);
                result[proces_num].send_num++;
            }
        }
    }

    return result;
}

void communicator_get_max_temp_delta(double *max_delta, double *temp, MPI_Comm comm, int root)
{

    MPI_Reduce(temp, max_delta, 1, MPI_DOUBLE, MPI_MAX, root, comm);

    MPI_Bcast(max_delta, 1, MPI_DOUBLE, root, comm);
}

//通过index获得进程号
int communicator_get_process_num_by_index(int index, int cube_numb)
{
    int i;
    i = index / cube_numb;
    if (index % cube_numb == 0)
    {
        return i - 1;
    }
    return i;
}

// //返回相邻方块数目
// process_send_num *communicator_get_process_send_num_list(const struct arr_cube *arr_cube, const size_t size, const size_t process_size)
// {
//      //initialize
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
//                 int proces_num = communicator_get_process_num_by_index(arr_cube[i].connect[j], size);
//                 result[proces_num].send_num++;
//             }
//         }
//     }

//     return result;
//}

//创建三维数组
int ***create_three_dimen_arr(int m, int n, int t)
{
    int i = 0;
    int k = 0;
    int ***result = NULL;
    if ((m > 0) && (n > 0) && (t > 0))
    {
        int **pp = NULL;
        int *p = NULL;
        result = (int ***)malloc(m * sizeof(int **)); // key
        pp = (int **)malloc(m * n * sizeof(int *));   // key
        p = (int *)malloc(m * n * t * sizeof(int));   // key
        if ((result != NULL) && (pp != NULL) && (p != NULL))
        {
            for (i = 0; i < m; i++)
            {
                result[i] = pp + i * n; // 三维元素存二维地址
                for (k = 0; k < n; k++)
                {
                    result[i][k] = p + k * t; // // 二维元素存一维地址
                }
                p = p + n * t;
            }
        }
        else
        {
            free(result);
            free(pp);
            free(p);
            result = NULL;
            pp = NULL;
            p = NULL;
        }
    }
    return result;
}

//创建三维结构体数组
struct cube ***create_three_dimen_struct_cube(int m, int n, int t)
{
    int i = 0;
    int k = 0;
    struct cube ***result = NULL;
    if ((m > 0) && (n > 0) && (t > 0))
    {
        struct cube **pp = NULL;
        struct cube *p = NULL;
        result = (struct cube ***)malloc(m * sizeof(struct cube **)); // key
        pp = (struct cube **)malloc(m * n * sizeof(struct cube *));   // key
        p = (struct cube *)malloc(m * n * t * sizeof(struct cube));   // key
        if ((result != NULL) && (pp != NULL) && (p != NULL))
        {
            for (i = 0; i < m; i++)
            {
                result[i] = pp + i * n; // 三维元素存二维地址
                for (k = 0; k < n; k++)
                {
                    result[i][k] = p + k * t; // // 二维元素存一维地址
                }
                p = p + n * t;
            }
        }
        else
        {
            free(result);
            free(pp);
            free(p);
            result = NULL;
            pp = NULL;
            p = NULL;
        }
    }
    return result;
}

//填充三维结构体数组
int fill_arr(struct cube ***temp, int ***arr)
{

    int temp_index = 1;
    double temp_tempa = 1.0;
    for (int i = 0; i < x_length; i++)
    {
        for (int j = 0; j < y_length; j++)
        {
            for (int k = 0; k < z_length; k++)
            {

                //假如是内部方块
                if (arr[i][j][k] == 1)
                {
                    temp[i][j][k].x = i;
                    temp[i][j][k].y = j;
                    temp[i][j][k].z = k;
                    temp[i][j][k].index = temp_index;
                    temp[i][j][k].tempa = 1000;
                    ++temp_index;
                    temp_tempa += 1.0;
                }
                //外部方块（环境方块）
                else
                {
                    temp[i][j][k].x = -1;
                    temp[i][j][k].y = -1;
                    temp[i][j][k].z = -1;
                    temp[i][j][k].index = -1;
                    temp[i][j][k].tempa = -0.1;
                }
                //printf("temp:x%d,\ntemp:y%d,\ntemp:z%d,\ntemp:index%d,\ntemp:tempa%f,\n",temp[i][j][k].x,temp[i][j][k].y,temp[i][j][k].z,temp[i][j][k].index,temp[i][j][k].tempa);
            }
        }
    }
    return temp_index - 1;
}


//填充二维数组
struct arr_cube *fill_arr_of_cube(int x, int y, int z, int myrank, int cube_numb_of_pro, struct cube ***temp)
{

    struct arr_cube *myarr = malloc(sizeof(struct arr_cube) * cube_numb_of_pro);
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            for (int k = 0; k < z; k++)
            {
                //填充该进程负责的myarr中的数据
                //假如某个方块属于该进程负责

                if ((temp[i][j][k].index > myrank * cube_numb_of_pro) && (temp[i][j][k].index <= (myrank * cube_numb_of_pro + cube_numb_of_pro)))
                {
                    int myarr_index;
                    if (temp[i][j][k].index == myrank * cube_numb_of_pro + cube_numb_of_pro)
                    {
                        myarr_index = temp[i][j][k].index % cube_numb_of_pro + cube_numb_of_pro - 1;
                    }
                    else
                    {
                        myarr_index = temp[i][j][k].index % cube_numb_of_pro - 1;
                    }
                    //将该方块放置其对应位置
                    myarr[myarr_index].temp = temp[i][j][k];

                    /*填充方块的相邻关系index，若某方块的index为-1，则该方块为外部方块*/
                    //8个角
                    if (i == 0 && j == 0 && k == 0)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = -1;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = -1;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = -1;
                    }
                    else if (i == 0 && j == 0 && k == z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = -1;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = -1;
                        myarr[myarr_index].connect[4] = -1;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (i == 0 && j == y_length - 1 && k == 0)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = -1;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = -1;
                    }
                    else if (i == 0 && j == y_length - 1 && k == z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = -1;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = -1;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (i == x_length - 1 && j == 0 && k == 0)
                    {
                        myarr[myarr_index].connect[0] = -1;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = -1;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = -1;
                    }
                    else if (i == x_length - 1 && j == 0 && k == z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = -1;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = -1;
                        myarr[myarr_index].connect[4] = -1;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (i == x_length - 1 && j == y_length - 1 && k == 0)
                    {
                        myarr[myarr_index].connect[0] = -1;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = -1;
                    }
                    else if (i == x_length - 1 && j == y_length - 1 && k == z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = -1;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = -1;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    //12条棱长
                    else if (i == 0 && j == 0 && k != 0 && k != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = -1;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = -1;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (i == x_length - 1 && j == 0 && k != 0 && k != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = -1;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = -1;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (i == x_length - 1 && j == y_length - 1 && k != 0 && k != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = -1;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (i == 0 && j == y_length - 1 && k != 0 && k != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = -1;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (k == 0 && j == 0 && i != 0 && i != x_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = -1;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = -1;
                    }

                    else if (k == 0 && j == y_length - 1 && i != 0 && i != x_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = -1;
                    }
                    else if (k == z_length - 1 && j == 0 && i != 0 && i != x_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = -1;
                        myarr[myarr_index].connect[4] = -1;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (k == z_length - 1 && j == y_length - 1 && i != 0 && i != x_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = -1;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (i == 0 && k == 0 && j != 0 && j != y_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = -1;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = -1;
                    }
                    else if (i == x_length - 1 && k == 0 && j != 0 && j != y_length - 1)
                    {
                        myarr[myarr_index].connect[0] = -1;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = -1;
                    }
                    else if (i == 0 && k == z_length - 1 && j != 0 && j != y_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = -1;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = -1;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (i == x_length - 1 && k == z_length - 1 && j != 0 && j != y_length - 1)
                    {
                        myarr[myarr_index].connect[0] = -1;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    //6个面
                    else if (i == 0 && j != 0 && j != y_length - 1 && k != 0 && k != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = -1;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (i == x_length - 1 && j != 0 && j != y_length - 1 && k != 0 && k != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = -1;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (j == 0 && i != 0 && i != x_length - 1 && k != 0 && k != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = -1;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (j == y_length - 1 && i != 0 && i != y_length - 1 && k != 0 && k != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = -1;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else if (k == 0 && j != 0 && j != y_length - 1 && i != 0 && i != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = -1;
                    }
                    else if (k == z_length - 1 && j != 0 && j != y_length - 1 && i != 0 && i != z_length - 1)
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = -1;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                    else
                    {
                        myarr[myarr_index].connect[0] = temp[i + 1][j][k].index;
                        myarr[myarr_index].connect[1] = temp[i - 1][j][k].index;
                        myarr[myarr_index].connect[2] = temp[i][j + 1][k].index;
                        myarr[myarr_index].connect[3] = temp[i][j - 1][k].index;
                        myarr[myarr_index].connect[4] = temp[i][j][k + 1].index;
                        myarr[myarr_index].connect[5] = temp[i][j][k - 1].index;
                    }
                }
            }
        }
    }
    return myarr;
}

//填充发送表
struct send_table_element *create_send_table(int allrank, int cube_numb_of_pro, process_send_num *con_arr, struct arr_cube *myarr)
{
    struct send_table_element *send_arr = malloc(allrank * sizeof(struct send_table_element));
    for (int i = 0; i < allrank; i++)
    {
        send_arr[i].pro_t = i;
        send_arr[i].element_numb = con_arr[i].send_num;
        send_arr[i].current_element_numb = 0;
        send_arr[i].element = malloc(send_arr[i].element_numb * sizeof(struct send_element));
    }
    //send_table建立完毕
    return send_arr;
}

//填充发送表
void fill_send_table(struct send_table_element *send_arr, struct arr_cube *myarr, int cube_numb_of_pro, int allrank)
{
    int pro_numb; //该方块属于的进程号
    for (int i = 0; i < cube_numb_of_pro; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            int connect_temp = myarr[i].connect[j]; //某个相邻方块的index
            //该邻接方块为内部方块
            if (connect_temp != -1)
            {
                double tempar = myarr[i].temp.tempa; //该方块的温度
                pro_numb = communicator_get_process_num_by_index(connect_temp, cube_numb_of_pro);
                send_arr[pro_numb].element[send_arr[pro_numb].current_element_numb].index = myarr[i].temp.index;
                send_arr[pro_numb].element[send_arr[pro_numb].current_element_numb].tempa = myarr[i].temp.tempa;
                send_arr[pro_numb].current_element_numb++;
            }
        }
    }
    for (int i = 0; i < allrank; i++)
        send_arr[i].current_element_numb = 0;
}

//新建MPI_Datatype
void commit_new_type(MPI_Datatype *MPI_send_element)
{
    struct send_element send_temp;
    // MPI_Datatype  MPI_send_element;

    MPI_Aint *disp = malloc(2 * sizeof(MPI_AINT));
    const MPI_Aint *disp1 = disp;

    const int block1[2] = {1, 1};
    const int *block = block1;

    const MPI_Datatype type[2] = {MPI_INT, MPI_DOUBLE};
    const MPI_Datatype *type1 = type;
    MPI_Get_address(&send_temp.index, &disp[0]);
    MPI_Get_address(&send_temp.tempa, &disp[1]);
    disp[1] = disp[1] - disp[0];
    disp[0] = 0;
    MPI_Type_create_struct(2, block, disp1, type1, MPI_send_element);
    MPI_Type_commit(MPI_send_element);
}

//通信
void communication(int allrank, int myrank, int cube_numb_of_pro, struct send_table_element *send_arr, process_send_num *con_arr, MPI_Datatype MPI_send_element, HashMap *index_temp_table)
{
    for (int i = 0; i < allrank; i++)
    {
        if (myrank == i) //轮到第i号进程发送数据
        {
            for (int j = 0; j < allrank; j++) //第i号进程向第j号进程发送数据
            {
                if (myrank != j)
                {
                    MPI_Send(send_arr[j].element, con_arr[j].send_num, MPI_send_element, j, i, MPI_COMM_WORLD);
                }
            }
        }
        else //其余进程负责接受send_table,
        {
            MPI_Status status;
            struct send_element *recv = malloc(cube_numb_of_pro * sizeof(struct send_element));
            MPI_Recv(recv, con_arr[i].send_num, MPI_send_element, i, i, MPI_COMM_WORLD, &status);
            //更新index-temp table

            int numb; //此次通信获取的MPI_send_element的数量
            MPI_Get_count(&status, MPI_send_element, &numb);

            for (int i = 0; i < numb; i++)
            {
                printf("rank[%d], update index[%d] to temp<%f>\n", myrank, recv[i].index, recv[i].tempa);
                *(double *)HashMap_get(index_temp_table, &recv[i].index) = recv[i].tempa;
            }


            //index-table更新完毕
            // printf("%d\n",numb);
            // if(numb!=0)
            // printf("process:%d\n%d\n%f\n",myrank,recv[1].index,recv[1].tempa);
            free(recv);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}
