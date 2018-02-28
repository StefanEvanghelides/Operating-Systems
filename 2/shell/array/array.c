#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

/* Initialized an array with maxSize = size. */
void initArray(Array *array) {
	array->length = 0;
	array->maxSize = 1;
	array->data = malloc(sizeof(char));
}


void initArrayList(ArrayList *arrayList) {
	arrayList->length = 0;
	arrayList->maxSize = 1;
	arrayList->data = malloc(sizeof(ArrayList));
	initArray(&(arrayList->data[0]));
}

/* Doubles the maximum size of the array. */
void doubleSize(Array *array) {
	int newSize = array->maxSize * 2;
	array->data  = realloc(array->data, newSize*sizeof(char));	
	array->maxSize = newSize;
}

void doubleSizeList(ArrayList *arrayList) {
	int newSize = arrayList->maxSize * 2;
	arrayList->data  = realloc(arrayList->data, newSize*sizeof(ArrayList));	
	arrayList->maxSize = newSize;
}

/* Adds an element to the array. If the array is full, its size is doubled. */
void addElement(Array *array, char element) {
	if(array->length == array->maxSize) {
		doubleSize(array);
	}
	array->data[array->length] = element;
	array->length++;
}


void addElementList(ArrayList *arrayList, Array element) {
	if(arrayList->length == arrayList->maxSize) {
		doubleSizeList(arrayList);
	}
	arrayList->data[arrayList->length] = element;
	arrayList->length++;
}

char **getArrayListData(ArrayList list) {
	char** data = malloc((list.length) * sizeof(char*));
	for(int i=0; i < list.length; i++) {
		if(list.data[i].data != NULL && strcmp(list.data[i].data, "")) {
			data[i] = list.data[i].data;
		} else if(list.data[i].data == NULL) {
			data[i] = NULL;
			break;
		}
	}
	return data;
}

void printArrayList(ArrayList list) {
	for(int i=0; i < list.length; i++) {
		printf("Array[%d]=", i);
		for(int j=0; j<list.data[i].length; j++) {
			printf("%d ", list.data[i].data[j]);
		}
		putchar('\n');
	}
}