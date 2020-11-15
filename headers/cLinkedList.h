#pragma once


#include <stdlib.h>



#ifndef _CLINKEDLIST_H_
#define _CLINKEDLIST_H_

/**
 * @brief store element by node, create list by cLinkedList_create(int nodeSize, int (*searchMethod)(const void* key, const void* content))
 * 
 */
typedef struct cLinkedList LinkedList;

/**
 * @brief linked list node, create node by cLinkedListNode_create(const void* content)
 * 
 */
typedef struct cLinkedListNode LinkedListNode;

/**
 * @brief linked list node
 * 
 */
struct cLinkedListNode
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
struct cLinkedList
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
 * @return cLinkedList 
 */
extern LinkedList cLinkedList_create(int (*comparter)(const void *contentA, const void *contentB));

/**
 * @brief create linked list node
 * 
 * @param content 
 * @return cLinkedListNode 
 */
extern LinkedListNode* cLinkedListNode_create(const void *content);

/**
 * @brief search node by content
 * 
 * @param content 
 * @return cLinkedListNode* 
 */
extern LinkedListNode *cLinkedList_search(const LinkedList *linkedList, const void *content);

/**
 * @brief add a node with content to the end of the linkedList
 * 
 * @param linkedList 
 * @param content 
 */
extern void cLinkedList_add(LinkedList *linkedList, const void *content);

/**
 * @brief remove node by content
 * 
 * @param linkedList 
 * @param content 
 */
extern void cLinkedList_removeByContent(LinkedList *linkedList, const void *content);

/**
 * @brief repalce node by content
 * 
 * @param linkedList 
 * @param newContent 
 * @param oldContent 
 */
extern void cLinkedList_replaceByContent(LinkedList *linkedList, const void *newContent, const void *oldContent);

/**
 * @brief free list
 * 
 * @param linkedList 
 */
extern void cLinkedList_destroy(LinkedList *linkedList);

/**
 * @brief free list node
 * 
 * @param linkedListNode 
 */
extern void cLinkedListNode_destroy(LinkedListNode *linkedListNode);

#endif