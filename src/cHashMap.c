#include "cHashMap.h"

typedef struct Pair Pair;
struct Pair
{
    const void *key;
    const void *content;
    const int *sizeof_key;
};

/**
 * @brief rehashing to keep O(1)
 * 
 * @param hashMap 
 */
void reHashing(HashMap *hashMap);

/**
 * @brief Get the Bucket of key
 * 
 * @param hashMap 
 * @param key 
 * @return LinkedList* 
 */
LinkedList *getBucket(const HashMap *hashMap, const void *key);

/**
 * @brief create hashmap with more capacity
 * 
 * @param hashMap 
 * @return HashMap 
 */
HashMap HashMap_create_new(HashMap *hashMap);

/**
 * @brief add pair to new hashmap
 * 
 * @param hashMap 
 * @param pair 
 */
void HashMap_addPair(HashMap *hashMap, Pair *pair);

int comparter(const void *contentA, const void *contentB)
{

    Pair pair_A = *(Pair *)contentA;

    Pair pair_B = *(Pair *)contentB;

    if (memcmp(pair_A.key, pair_B.key, *pair_A.sizeof_key) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// HashMap HashMap_create(int (*comparter)(const void *contentA, const void *contentB), int (*hash)(const void *content))
// {

//     HashMap hashMap = {
//         .hash = hash,
//         .list = LinkedList_create(comparter)};

//     return hashMap;
// }

HashMap HashMap_create(int (*hash)(const void *content), const int sizeof_key, const float LOAD_FACTOR)
{

    HashMap hashMap = {
        .hash = hash,
        .list = ArrayList_create_bysize(sizeof(LinkedList), 8),
        .sizeof_key = sizeof_key,
        .LOAD_FACTOR = LOAD_FACTOR,
        .current_size = 0};

    for (size_t i = 0; i < hashMap.list.size; i++)
    {
        LinkedList linkedList = LinkedList_create(comparter);
        ArrayList_add(&hashMap.list, &linkedList);
    }

    return hashMap;
}

void HashMap_put(HashMap *hashMap, const void *key, const void *value)
{
    struct Pair *pair = malloc(sizeof(struct Pair));

    pair->key = key;
    pair->sizeof_key = &hashMap->sizeof_key;
    pair->content = value;

    LinkedList *buket = getBucket(hashMap, pair->key);

    LinkedListNode *node = LinkedList_search(buket, pair);

    if (node == NULL)
    {
        //printf("put add\n");
        LinkedList_add(buket, pair);
        hashMap->current_size++;

        reHashing(hashMap);
    }
}

void *HashMap_get(const HashMap *hashMap, const void *key)
{

    struct Pair pair = {
        .key = key,
        .sizeof_key = &hashMap->sizeof_key};

    LinkedList *buket = getBucket(hashMap, pair.key);
    LinkedListNode *node = LinkedList_search(buket, &pair);

    return (void *)((Pair *)node->content)->content;
}

void HashMap_repalce(HashMap *hashMap, const void *key, const void *value)
{

    struct Pair pair = {
        .key = key,
        .sizeof_key = &hashMap->sizeof_key};

    LinkedList *buket = getBucket(hashMap, pair.key);

    LinkedListNode *node = LinkedList_search(buket, &pair);

    if (node != NULL)
    {
        ((Pair *)node->content)->content = (void *)value;
    }
}

void HashMap_destroy(HashMap *hashMap)
{

    for (size_t i = 0; i < hashMap->list.current_size; i++)
    {
        LinkedList *buket = (LinkedList *)ArrayList_get(&hashMap->list, i);
        LinkedList_destroy(buket);
    }

    ArrayList_destroy(&hashMap->list);
}

/****************
    private
****************/

void reHashing(HashMap *hashMap)
{

    //filled bucket num/bucket num
    while (hashMap->LOAD_FACTOR < ((float)hashMap->current_size / (float)hashMap->list.current_size))
    {
        HashMap new_hashMap = HashMap_create_new(hashMap);

        ArrayList_destroy(&hashMap->list);

        hashMap->list = new_hashMap.list;
    }
}

LinkedList *getBucket(const HashMap *hashMap, const void *key)
{

    int index = hashMap->hash(key) % hashMap->list.current_size;

    LinkedList *buket = (LinkedList *)ArrayList_get(&hashMap->list, index);

    return buket;
}



HashMap HashMap_create_new(HashMap *hashMap)
{

    HashMap new_HashMap = {
        .hash = hashMap->hash,
        .list = ArrayList_create_bysize(sizeof(LinkedList), hashMap->list.size * 2),
        .sizeof_key = hashMap->current_size,
        .LOAD_FACTOR = hashMap->LOAD_FACTOR,
        .current_size = 0};

    for (size_t i = 0; i < new_HashMap.list.size; i++)
    {
        LinkedList linkedList = LinkedList_create(comparter);
        ArrayList_add(&new_HashMap.list, &linkedList);
    }

    for (size_t i = 0; i < hashMap->list.size; i++)
    {

        LinkedList *buket = (LinkedList *)ArrayList_get(&hashMap->list, i);

        for (size_t j = 0; j < buket->size; j++)
        {
            Pair *pair = (Pair *)LinkedList_getByIndex(buket, j);
            HashMap_addPair(&new_HashMap, pair);
        }
    }


    
    return new_HashMap;
}

void HashMap_addPair(HashMap *hashMap, Pair *pair)
{

    LinkedList *buket = getBucket(hashMap, pair->key);

    LinkedListNode *node = LinkedList_search(buket, pair);

    if (node == NULL)
    {
        LinkedList_add(buket, pair);
        hashMap->current_size++;
    }
}