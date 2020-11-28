#include "heat_conduct.h"

double env_temp = 25.0;

int hash_getInt(const void *content)
{
    int key = *(int *)content;
    return *(int *)content;
}

HashMap heat_conduct_create_index_temp_table(arr_cube *arr_cube, const size_t arr_cube_length)
{

    HashMap index_temp_table = HashMap_create(&hash_getInt, sizeof(int), 0.75);

    for (int i = 0; i < arr_cube_length; i++)
    {

        for (int j = 0; j < 6; j++)
        {
            if (arr_cube[i].connect[j] != -1)
            {

                int *index = malloc(sizeof(int));

                *index = arr_cube[i].connect[j];

                double *temp = malloc(sizeof(double));
                *temp = (double)arr_cube[i].connect[j];

                HashMap_put(&index_temp_table, index, temp);
            }
        }

        // for (int i = 0; i < 10; i++)
        // {

        //     int *index = malloc(sizeof(int));
        //     *index = i;
        //     double *value = (double *)HashMap_get(&index_temp_table, index);

        //     if (value != NULL)
        //     {
        //         printf("indeex:%i, temp: %f\n", i, *value);
        //     }
        // }
    }

    return index_temp_table;
}

double heat_conduct_play(arr_cube *arr_cube, const size_t arr_cube_length, HashMap *index_temp_table, int rank)
{

    double max_change = 0;

    //HashMap new_index_temp_table = HashMap_create(&hash_getInt, sizeof(int), 0.75);

    LinkedList result = LinkedList_create(NULL);

    for (int i = 0; i < arr_cube_length; i++)
    {
        double *temp_sum = malloc(sizeof(double));
        *temp_sum = 0;


       printf("\n[rank: %d],for index:[%d]\n", rank, arr_cube[i].temp.index);
        //sum of connect cube temperature
        for (int j = 0; j < 6; j++)
        {
            if (arr_cube[i].connect[j] != -1)
            {
                double *value = (double *)HashMap_get(index_temp_table, &arr_cube[i].connect[j]);

                if (value == NULL)
                {
                    printf("[rank: %d],element: <%d>,content: <not found>\n", rank, arr_cube[i].connect[j]);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    *temp_sum += *(double *)HashMap_get(index_temp_table, &arr_cube[i].connect[j]);
                    printf("[rank: %d],+<%d>%f", rank, arr_cube[i].connect[j], *(double *)HashMap_get(index_temp_table, &arr_cube[i].connect[j]));
                }
            }
            else
            {
                *temp_sum += env_temp;
            }
        }
        printf("\n[rank: %d],sum:<%f>", rank, arr_cube[i].temp.index, *temp_sum);
        *temp_sum = *temp_sum / 6;
        printf("\n[rank: %d],averange:<%f>", rank, arr_cube[i].temp.index, *temp_sum);
        //printf("%f - %f\n", arr_cube[i].temp.tempa, *temp_sum);

        double delta = arr_cube[i].temp.tempa - *temp_sum;

        if (delta < 0)
        {
            delta = -delta;
        }

        if (delta > max_change)
        {
            max_change = delta;
        }

       // printf("rank [%d], index: <%d>, temp<%f>to<%f>\n", rank, arr_cube[i].temp.index, arr_cube[i].temp.tempa, *temp_sum);

        //update cube temp
        arr_cube[i].temp.tempa = *temp_sum;

        //put into new index-temp table
        //HashMap_put(&new_index_temp_table, &arr_cube[i].temp.index, temp_sum);

        send_element *s = malloc(sizeof(send_element));

        s->index = arr_cube[i].temp.index;
        s->tempa = *temp_sum;

        LinkedList_add(&result, s);
    }

    LinkedListNode *head = result.headNode;

    while (head != NULL)
    {
        int index = ((send_element *)head->content)->index;
        double *value = (double *)HashMap_get(index_temp_table, &index);

        double new_value = ((send_element *)head->content)->tempa;

        if (value != NULL)
        {
            *value = new_value;
        }

        head = head->next;
    }

    // for (size_t i = 0; i < arr_cube_length; i++){

    //         HashMap_get(index_temp_table, arr_cube[i])

    // }

    //HashMap_destroy(index_temp_table);

    //index_temp_table = &new_index_temp_table;

    return max_change;
}

int heat_conduct_isDone(double value)
{

    if (value < 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}