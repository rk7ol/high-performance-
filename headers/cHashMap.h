#include "cLinkedList.h"


#ifndef _CHASHMAP_H_
#define _CHASHMAP_H_


/**
 * @brief hash map, create hash map by cHashMap_create()
 * 
 */
typedef struct cHashMap HashMap;

/**
 * @brief hash map
 * 
 */
struct cHashMap
{

    int (*hash)(const void* content);


    LinkedList list;

};



// /**
//  * @brief 
//  * 
//  * @param comparter comparer A and B
//  * @param hash 
//  * @return HashMap 
//  */
// extern HashMap cHashMap_create(int (*comparter)(const void *contentA, const void *contentB), int (*hash)(const void* content));

/**
 * @brief create hash map
 * 
 * @param hash 
 * @return HashMap 
 */
extern HashMap cHashMap_create(int (*hash)(const void* content));

/**
 * @brief put key-value to hash map
 * 
 * @param hashMap 
 * @param key 
 * @param value 
 */
extern void cHashMap_put(HashMap* hashMap, const void* key, const void* value);

/**
 * @brief get value of key
 * 
 * @param hashMap 
 * @param key 
 * @return void* 
 */
extern void* cHashMap_get(const HashMap* hashMap, const void* key);

/**
 * @brief replace with new value
 * 
 * @param hashMap 
 * @param key 
 * @param value 
 */
extern void cHashMap_repalce(HashMap* hashMap, const void* key, const void* value);





#endif