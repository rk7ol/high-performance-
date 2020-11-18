#include <stdio.h>
#include <string.h>

#include "cHashMap.h"
#include "cArrayList.h"
#include "util.h"

int cube_temp_hash(const void *content)
{

    cube_temp *c = (cube_temp *)content;

    return c->index;
}

void arrayList_example()
{

    //create
    ArrayList arrayList = ArrayList_create(sizeof(int));

    //add
    for (size_t i = 0; i < 100; i++)
    {
        ArrayList_add(&arrayList, &i);
    }

    //get and print
    for (size_t i = 0; i < arrayList.current_size; i++)
    {
        printf("value: %d\n", *(int *)ArrayList_get(&arrayList, i));
    }

    //free
    ArrayList_destroy(&arrayList);
}

void hashMap_example()
{

    //while (1)
    //{
        //create
        HashMap hashMap = HashMap_create(&cube_temp_hash, sizeof(int), 0.75);

        //put
        for (size_t i = 0; i < 10000; i++)
        {
            cube_temp *c1 = malloc(sizeof(cube_temp));

            c1->index = i;
            c1->temp = i + 1.0;

            HashMap_put(&hashMap, c1, c1);
        }


        //replace
        cube_temp *c2 = malloc(sizeof(cube_temp));

        c2->index = 40;
        c2->temp = 8000.888;

        HashMap_repalce(&hashMap, c2, c2);


        //get
        for (size_t i = 0; i < 100; i++)
        {
            cube_temp c1 = {
                .index = i,
                .temp = i + 1.0};
            cube_temp *cube = (cube_temp *)HashMap_get(&hashMap, &c1);

            printf("out value:%d, %lf\n", cube->index, cube->temp);
        }

        HashMap_destroy(&hashMap);
    //}
}

int main(int argc, char *argv[])
{

    hashMap_example();

    //arrayList_example();

    return 0;
}