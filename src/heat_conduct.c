#include "heat_conduct.h"

double env_temp = 25.0;

int hash_getInt(const void *content)
{
    return *(int *)content;
}

HashMap heat_conduct_create_index_temp_table(arr_cube *arr_cube, const size_t arr_cube_length)
{

    HashMap index_temp_table = HashMap_create(&hash_getInt, sizeof(int), 0.75);

    for (size_t i = 0; i < arr_cube_length; i++)
    {
        double *temp = malloc(sizeof(double));
        *temp = arr_cube[i].temp.tempa;
        HashMap_put(&index_temp_table, &arr_cube[i].temp.index, temp);
    }

    return index_temp_table;
}

void heat_conduct_play(arr_cube *arr_cube, const size_t arr_cube_length, HashMap *index_temp_table)
{

    HashMap new_index_temp_table = HashMap_create(&hash_getInt, sizeof(int), 0.75);

    for (size_t i = 0; i < arr_cube_length; i++)
    {
        double *temp_sum = malloc(sizeof(double));
        *temp_sum = 0;

        //sum of connect cube temperature
        for (size_t j = 0; j < 6; j++)
        {
            if (arr_cube[i].connect[i] != -1)
            {
                *temp_sum += *(double *)HashMap_get(index_temp_table, &arr_cube[i].connect[i]);
            }
            else
            {
                *temp_sum += env_temp;
            }
        }

        //update cube temp
        arr_cube->temp.tempa = *temp_sum;

        //put into new index-temp table
        HashMap_put(&index_temp_table, &arr_cube[i].temp.index, temp_sum);
    }

    HashMap_destroy(index_temp_table);

    index_temp_table = &new_index_temp_table;
}