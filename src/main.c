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
        printf("value: %d\n", *(int*)ArrayList_get(&arrayList, i));
    }

    //free
    ArrayList_destroy(&arrayList);
}

void hashMap_example()
{
    //create
    HashMap hashMap = HashMap_create(&cube_temp_hash);

    //put
    cube_temp c1 = {
        .index = 100,
        .temp = 50.0};

    cube_temp c2 = {
        .index = 50,
        .temp = 257.4};

    cube_temp c3 = {
        .index = 77,
        .temp = 35.5};

    HashMap_put(&hashMap, &c1, &c1);
    HashMap_put(&hashMap, &c2, &c2);
    HashMap_put(&hashMap, &c3, &c3);

    //get
    cube_temp recv = {
        .index = 100};
    recv = *(cube_temp *)HashMap_get(&hashMap, &recv);
    printf("temp: %f\n", recv.temp);

    //repalce
    cube_temp c5 = {
        .temp = 200};

    HashMap_repalce(&hashMap, &recv, &c5);
    recv = *(cube_temp *)HashMap_get(&hashMap, &recv);

    printf("temp: %f\n", recv.temp);
}

int main(int argc, char *argv[])
{

    //hashMap_example();

    arrayList_example();

    return 0;
}