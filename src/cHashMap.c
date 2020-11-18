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

// HashMap HashMap_create(int (*comparter)(const void *contentA, const void *contentB), int (*hash)(const void *content))
// {

//     HashMap hashMap = {
//         .hash = hash,
//         .list = LinkedList_create(comparter)};

//     return hashMap;
// }

HashMap HashMap_create(int (*hash)(const void *content))
{

    HashMap hashMap = {
        .hash = hash,
        .list = LinkedList_create(comparter)};

    return hashMap;
}

void HashMap_put(HashMap *hashMap, const void *key, const void *value)
{

    struct Pair *pair = malloc(sizeof(struct Pair));

    pair->key = hashMap->hash(key);

    pair->content = (void *)value;

    LinkedListNode *node = LinkedList_search(&hashMap->list, pair);

    if (node == NULL)
    {
        LinkedList_add(&hashMap->list, pair);
    }
}

void *HashMap_get(const HashMap *hashMap, const void *key)
{

    struct Pair *pair = malloc(sizeof(struct Pair));

    pair->key = hashMap->hash(key);

    LinkedListNode *node = LinkedList_search(&hashMap->list, pair);

    return ((Pair *)node->content)->content;
}

void HashMap_repalce(HashMap *hashMap, const void *key, const void *value)
{

    struct Pair *pair = malloc(sizeof(struct Pair));

    pair->key = hashMap->hash(key);

    LinkedListNode *node = LinkedList_search(&hashMap->list, pair);

    if (node != NULL)
    {
        ((Pair *)node->content)->content = (void *)value;
    }
}


void HashMap_destroy(HashMap* hashMap){

    LinkedList_destroy(&hashMap->list);

    


}