#include "cHashMap.h"

typedef struct Pair Pair;
struct Pair
{
    int key;
    void *content;
};

int comparter(const void *contentA, const void *contentB)
{

    Pair pair_A = *(Pair *)contentA;

    Pair pair_B = *(Pair *)contentB;

    if (pair_A.key == pair_B.key)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// HashMap cHashMap_create(int (*comparter)(const void *contentA, const void *contentB), int (*hash)(const void *content))
// {

//     HashMap hashMap = {
//         .hash = hash,
//         .list = cLinkedList_create(comparter)};

//     return hashMap;
// }

HashMap cHashMap_create(int (*hash)(const void *content))
{

    HashMap hashMap = {
        .hash = hash,
        .list = cLinkedList_create(comparter)};

    return hashMap;
}

void cHashMap_put(HashMap *hashMap, const void *key, const void *value)
{

    struct Pair *pair = malloc(sizeof(struct Pair));

    pair->key = hashMap->hash(key);

    pair->content = (void *)value;

    LinkedListNode *node = cLinkedList_search(&hashMap->list, pair);

    if (node == NULL)
    {
        cLinkedList_add(&hashMap->list, pair);
    }
}

void *cHashMap_get(const HashMap *hashMap, const void *key)
{

    struct Pair *pair = malloc(sizeof(struct Pair));

    pair->key = hashMap->hash(key);

    LinkedListNode *node = cLinkedList_search(&hashMap->list, pair);

    return ((Pair *)node->content)->content;
}

void cHashMap_repalce(HashMap *hashMap, const void *key, const void *value)
{

    struct Pair *pair = malloc(sizeof(struct Pair));

    pair->key = hashMap->hash(key);

    LinkedListNode *node = cLinkedList_search(&hashMap->list, pair);

    if (node != NULL)
    {
        ((Pair *)node->content)->content = (void *)value;
    }
}