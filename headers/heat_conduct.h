

#ifndef _HEAT_CONDUCT_H_
#define _HEAT_CONDUCT_H_

#include "cHashMap.h"
#include "communicator.h"





/**
 * @brief create index-temp table by cube array for each process
 * 
 * @param arr_cube 
 * @return HashMap 
 */
extern HashMap heat_conduct_create_index_temp_table(arr_cube* arr_cube, const size_t arr_cube_length);


/**
 * @brief play heat conduct, index-temp table will change to new state
 * 
 * @param arr_cube 
 * @param index_temp_table 
 */
extern void heat_conduct_play(arr_cube* arr_cube, const size_t arr_cube_length, HashMap* index_temp_table);







#endif