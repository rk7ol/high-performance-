#include <stdio.h>
#include <string.h>

#include "cHashMap.h"
#include "util.h"

int cube_temp_hash(const void *content)
{

    cube_temp *c = (cube_temp *)content;

    return c->index;
}

void hashMap_example()
{
    //create
    HashMap hashMap = cHashMap_create(&cube_temp_hash);

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

    cHashMap_put(&hashMap, &c1, &c1);
    cHashMap_put(&hashMap, &c2, &c2);
    cHashMap_put(&hashMap, &c3, &c3);


    //get
    cube_temp recv = {
        .index = 100
    };
    recv = *(cube_temp*)cHashMap_get(&hashMap, &recv);
    printf("temp: %f\n", recv.temp);



    //repalce
    cube_temp c5 = {
        .temp = 200};

    cHashMap_repalce(&hashMap, &recv, &c5);
    recv = *(cube_temp*)cHashMap_get(&hashMap, &recv);

    printf("temp: %f\n", recv.temp);



}

int main(int argc, char *argv[])
{

    hashMap_example();

    return 0;
}