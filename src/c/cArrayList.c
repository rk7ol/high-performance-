#include "cArrayList.h"



ArrayList ArrayList_create(int element_size){

	ArrayList  list = {
		.current_size = 0,
		.element_size = element_size,
		.size = 0,
		.content = 0
	};

	return list;

}


ArrayList ArrayList_create_bysize(int element_size, int size){

	ArrayList list = {
		.current_size = 0,
		.element_size = element_size,
		.size = size,
		.content = malloc(size * element_size)
	};

	return list;

}




void ArrayList_add(ArrayList* list, const void* element) {


	if (list->content == 0)
	{
		list->content = malloc(8 * list->element_size);

		//assign mem fail
		if (list->content == 0) {
			exit(EXIT_FAILURE);
		}


		memset(list->content, 0, 8 * list->element_size);
		


		list->size = 8;
	}


	if (list->current_size == list->size) {

		//space not enough

		//char * temp = realloc(list->content, list->size * 8 * list->element_size);

		char* temp = malloc(list->size * 8 * list->element_size);



		if (temp != 0) {
			memset(temp, 0, list->size * 8 * list->element_size);
			memcpy(temp, list->content, list->current_size * list->element_size);
			free(list->content);
			list->content = temp;
			list->size *= 8;
		} else{
			exit(EXIT_FAILURE);
		}

		ArrayList_add(list, element);

	} else{

		//space enough


		// for (int i = 0; i < list -> element_size; i++) {
		// 	list->content[i + list->current_size * list->element_size] = ((char*)element)[i];
		// }


		memcpy(&(list->content[list->current_size * list->element_size]), element, list->element_size);

		list->current_size++;

	}

}



const void* ArrayList_get(const ArrayList* list, int index) {

	//border check
	if (index < 0 || index > list->current_size) {


		exit(EXIT_FAILURE);
	}


	// char * temp = malloc(list->element_size);


	// for (size_t i = 0; i < list->element_size; i++) {


	// 	temp[i] = list->content[i + index * list->element_size];

	
	// }


	return &(list->content[index * list->element_size]);
	


	//return (void *)temp;
}


void ArrayList_repalce(ArrayList* list, int index, const void* element){


	//border check
	if (index < 0 || index > list->current_size) {

		exit(EXIT_FAILURE);
	}



	for (size_t i = 0; i < list->element_size; i++) {

		list->content[i + index * list->element_size] = ((char*)element)[i];

	}


}


void ArrayList_remove(ArrayList* list, int index) {

	//from index to end
	for (int i = index; i < list->current_size; i++) {

		//replace with next element
		for (int j = 0; j < list->element_size; j++) {

			list->content[i * list->element_size + j] = list->content[(i + 1) * list->element_size + j];
			
		}
		
	}


	list->current_size--;
	



}



void ArrayList_destroy(ArrayList* list){

	free(list->content);
	//free(list);

}
