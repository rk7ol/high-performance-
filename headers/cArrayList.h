#pragma once

#include <stdlib.h>
#include <string.h>


#ifndef _CARRAYLIST_H_
#define _CARRAYLIST_H_

/**
 * @brief  do not create yourself, create it by method ArrayList_create(int element_size)
 * 
 * 
 */
typedef  struct ArrayList_struct ArrayList;


/**
 * @brief store element in byte
 * 
 * 	
 * 
 */
struct ArrayList_struct {

	int size; //max size
	int current_size; //element count of list
	int element_size;//element size
	char* content; //content

};


/**
 * @brief  add element to list
 * 
 * @param list list to store element
 * @param element  element be added
 */
extern void ArrayList_add(ArrayList* list, const void* element);


/**
 * @brief get element from list by index
 * 
 * @param list element source
 * @param index element index
 * @return void* result element ptr
 */
extern const void* ArrayList_get(const ArrayList* list, int index);


/**
 * @brief replace element at index of list
 * 
 * 
 * @param list element source
 * @param index index of repalce
 * @param element element to replace
 */
extern void ArrayList_repalce(ArrayList* list, int index, const void* element);


/**
 * @brief remove element at index of list
 * 
 * 
 * @param list element source
 * @param index index of element to remove
 */
extern void ArrayList_remove(ArrayList* list, int index);



/**
 * @brief create a list
 * 
 * @param element_size size of element be stored
 * @return ArrayList* list be created
 */
extern ArrayList ArrayList_create(int element_size);


/**
 * @brief create a list with initilize size
 * 
 * @param element_size 
 * @param size 
 * @return ArrayList* 
 */
extern ArrayList ArrayList_create_bysize(int element_size, int size);

/**
 * @brief free the list
 * 
 * @param list 
 */
extern void ArrayList_destroy(ArrayList* list);




#endif