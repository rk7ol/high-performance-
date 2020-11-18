#include "cLinkedList.h"

LinkedList LinkedList_create(int (*comparter)(const void* content_A, const void *content_B))
{

    LinkedList linkedList = {
        .headNode = NULL,
        .tailNode = NULL,
        .comparter = comparter,
        .size = 0};

    return linkedList;
}

LinkedListNode* LinkedListNode_create(const void *content)
{

    LinkedListNode *linkedListNode = malloc(sizeof(LinkedListNode));


    linkedListNode->content = (void*)content;

    linkedListNode->next = NULL;
    linkedListNode->previous = NULL;


    return linkedListNode;
}

LinkedListNode *LinkedList_search(const LinkedList *linkedList, const void *key)
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

void LinkedList_add(LinkedList *linkedList, const void *content)
{

    LinkedListNode* node = LinkedListNode_create(content);
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

void LinkedList_removeByContent(LinkedList *linkedList, const void *key)
{
    LinkedListNode *node = LinkedList_search(linkedList, key);

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

        LinkedListNode_destroy(node);

        linkedList->size--;
    }
}

void LinkedList_replaceByContent(LinkedList *linkedList, const void *key, const void *content)
{

    LinkedListNode *node = LinkedList_search(linkedList, key);

    if (node != NULL)
    {
        //free previous
        free(node->content);

        //change to new content
        node->content = (void *)content;
    }
}

void LinkedList_destroy(LinkedList *linkedList)
{

    LinkedListNode *node = linkedList->headNode;

    LinkedListNode *nextNode;

    while (node != NULL)
    {
        nextNode = node->next;
        LinkedListNode_destroy(node);
        node = nextNode;
    }
}

void LinkedListNode_destroy(LinkedListNode *linkedListNode)
{

    //free content
    free(linkedListNode->content);

    // //point to 0
    // linkedListNode->next = NULL;
    // linkedListNode->previous = NULL;

    free(linkedListNode);
}