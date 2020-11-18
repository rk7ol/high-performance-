#pragma once


#include <stdlib.h>



#ifndef _CLINKEDLIST_H_
#define _CLINKEDLIST_H_

/**
 * @brief store element by node, create list by LinkedList_create(int nodeSize, int (*searchMethod)(const void* key, const void* content))
 * 
 */
typedef struct LinkedList LinkedList;

/**
 * @brief linked list node, create node by LinkedListNode_create(const void* content)
 * 
 */
typedef struct LinkedListNode LinkedListNode;

/**
 * @brief linked list node
 * 
 */
struct LinkedListNode
{

    char *content;
    //point to next node
    LinkedListNode *next;
    //point to previous node
    LinkedListNode *previous;
};

/**
 * @brief linked list
 * 
 */
struct LinkedList
{

    LinkedListNode *headNode;
    LinkedListNode *tailNode;
    int size;
    /**
     * @brief return 1, if content represent content, else 0
     * 
     */
    int (*comparter)(const void *contentA, const void *contentB);
};

/**
 * @brief create linked list with search method
 * 
 * @param nodeSize 
 * @param searchMethod 
 * @return LinkedList 
 */
extern LinkedList LinkedList_create(int (*comparter)(const void *contentA, const void *contentB));

/**
 * @brief create linked list node
 * 
 * @param content 
 * @return LinkedListNode 
 */
extern LinkedListNode* LinkedListNode_create(const void *content);

/**
 * @brief search node by content
 * 
 * @param content 
 * @return LinkedListNode* 
 */
extern LinkedListNode *LinkedList_search(const LinkedList *linkedList, const void *content);



/**
 * @brief get element by index
 * 
 * @param linkedList 
 * @param index 
 * @return char* 
 */
extern char* LinkedList_getByIndex(const LinkedList *linkedList, const int index);

/**
 * @brief add a node with content to the end of the linkedList
 * 
 * @param linkedList 
 * @param content 
 */
extern void LinkedList_add(LinkedList *linkedList, const void *content);

/**
 * @brief remove node by content
 * 
 * @param linkedList 
 * @param content 
 */
extern void LinkedList_removeByContent(LinkedList *linkedList, const void *content);

/**
 * @brief repalce node by content
 * 
 * @param linkedList 
 * @param newContent 
 * @param oldContent 
 */
extern void LinkedList_replaceByContent(LinkedList *linkedList, const void *newContent, const void *oldContent);

/**
 * @brief free list
 * 
 * @param linkedList 
 */
extern void LinkedList_destroy(LinkedList *linkedList);

/**
 * @brief free list node
 * 
 * @param linkedListNode 
 */
extern void LinkedListNode_destroy(LinkedListNode *linkedListNode);

#endif