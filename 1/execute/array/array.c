#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

/* Initialized an array with maxSize = size. */
void initArray(Array *array, int size) {
	array->length = 0;
	array->maxSize = size;
	array->data = calloc(size, sizeof(char*));
}

/* Doubles the maximum size of the array. */
void doubleSize(Array *array) {
	int newSize = array->maxSize * 2;
	array->data  = realloc(array->data, newSize*sizeof(char*));	
	array->maxSize = newSize;
}

/* Adds an element to the array. If the array is full, its size is doubled. */
void addElement(Array *array, char* element) {
	if(array->length == array->maxSize) {
		doubleSize(array);
	}
	array->data[array->length] = element;
	array->length++;
}

/* Free Array's memory. */
void freeArray(Array array) {
	free(array.data);
}
