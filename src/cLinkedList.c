#include "cLinkedList.h"

LinkedList cLinkedList_create(int (*comparter)(const void* content_A, const void *content_B))
{

    LinkedList linkedList = {
        .headNode = NULL,
        .tailNode = NULL,
        .comparter = comparter,
        .size = 0};

    return linkedList;
}

LinkedListNode* cLinkedListNode_create(const void *content)
{

    LinkedListNode *linkedListNode = malloc(sizeof(LinkedListNode));


    linkedListNode->content = (void*)content;

    linkedListNode->next = NULL;
    linkedListNode->previous = NULL;


    return linkedListNode;
}

LinkedListNode *cLinkedList_search(const LinkedList *linkedList, const void *key)
{

    if (linkedList->headNode == NULL)
    {
        return NULL;
    }
    else
    {
        LinkedListNode *current;
        current = linkedList->headNode;

        //if not null
        while (current != NULL)
        {
            if (linkedList->comparter(key, current->content))
            {
                //find
                return current;
            }
            else
            {
                //search next
                current = current->next;
            }
        }
    }

    //not find
    return NULL;
}

void cLinkedList_add(LinkedList *linkedList, const void *content)
{

    LinkedListNode* node = cLinkedListNode_create(content);
    if (linkedList->size == 0)
    {
        linkedList->headNode = node;
        linkedList->tailNode = node;
    }
    else
    {

        //add to tail
        linkedList->tailNode->next = node;
        node->previous = linkedList->tailNode;

        //change to tail
        linkedList->tailNode = node;
    }

    linkedList->size = linkedList->size + 1;
}

void cLinkedList_removeByContent(LinkedList *linkedList, const void *key)
{
    LinkedListNode *node = cLinkedList_search(linkedList, key);

    if (node != NULL)
    {
        //change head
        if (linkedList->headNode == node)
        {
            linkedList->headNode = linkedList->headNode->next;
        }

        if (linkedList->tailNode == node)
        {
            linkedList->tailNode = linkedList->tailNode->previous;
        }

        //remove from list
        node->previous->next = node->next;

        cLinkedListNode_destroy(node);

        linkedList->size--;
    }
}

void cLinkedList_replaceByContent(LinkedList *linkedList, const void *key, const void *content)
{

    LinkedListNode *node = cLinkedList_search(linkedList, key);

    if (node != NULL)
    {
        //free previous
        free(node->content);

        //change to new content
        node->content = (void *)content;
    }
}

void cLinkedList_destroy(LinkedList *linkedList)
{

    LinkedListNode *node = linkedList->headNode;

    LinkedListNode *nextNode;

    while (node != NULL)
    {
        nextNode = node->next;
        cLinkedListNode_destroy(node);
        node = nextNode;
    }
}

void cLinkedListNode_destroy(LinkedListNode *linkedListNode)
{

    //free content
    free(linkedListNode->content);

    // //point to 0
    // linkedListNode->next = NULL;
    // linkedListNode->previous = NULL;

    free(linkedListNode);
}