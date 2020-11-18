#include "cLinkedList.h"
#include "cArrayList.h"


#ifndef _CHASHMAP_H_
#define _CHASHMAP_H_


/**
 * @brief hash map, create hash map by HashMap_create()
 * 
 */
typedef struct HashMap HashMap;

/**
 * @brief hash map
 * 
 */
struct HashMap
{

    /**
     * @brief content provide key
     * 
     */
    int (*hash)(const void* content);


    ArrayList list;

    // key size
    const int sizeof_key;

    //load factor
    const float LOAD_FACTOR;

    int current_size;
};



// /**
//  * @brief 
//  * 
//  * @param comparter comparer A and B
//  * @param hash 
//  * @return HashMap 
//  */
// extern HashMap HashMap_create(int (*comparter)(const void *contentA, const void *contentB), int (*hash)(const void* content));

/**
 * @brief create hash map
 * 
 * @param hash 
 * @return HashMap 
 */
extern HashMap HashMap_create(int (*hash)(const void* content), const int sizeof_key, const float LOAD_FACTOR);

/**
 * @brief put key-value to hash map
 * 
 * @param hashMap 
 * @param key 
 * @param value 
 */
extern void HashMap_put(HashMap* hashMap, const void* key, const void* value);

/**
 * @brief get value of key
 * 
 * @param hashMap 
 * @param key 
 * @return void* 
 */
extern void* HashMap_get(const HashMap* hashMap, const void* key);

/**
 * @brief replace with new value
 * 
 * @param hashMap 
 * @param key 
 * @param value 
 */
extern void HashMap_repalce(HashMap* hashMap, const void* key, const void* value);




/**
 * @brief free hash map
 * 
 * @param hashMap 
 */
extern void HashMap_destroy(HashMap* hashMap);





#endif