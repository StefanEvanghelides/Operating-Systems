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

void initArray2D(Array2D *array2d, int rows, int cols) {
	array2d->length = 0;
	array2d->maxSize = rows;
	array2d->data = malloc(rows * sizeof(char*));
	for(int row=0; row<rows; row++) {
		array2d->data[row] = malloc(cols * sizeof(char));
	}
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

void doubleSize2D(Array2D *array2d) {
	int newSize = array2d->maxSize * 2;
	array2d->data  = realloc(array2d->data, newSize*sizeof(char *));	
	array2d->maxSize = newSize;
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

void addElement2D(Array2D *array2d, char *element) {
	if(array2d->length == array2d->maxSize) {
		doubleSize2D(array2d);
	}
	array2d->data[array2d->length] = element;
	array2d->length++;
}

void addElementList(ArrayList *arrayList, Array element) {
	if(arrayList->length == arrayList->maxSize) {
		doubleSizeList(arrayList);
	}
	arrayList->data[arrayList->length] = element;
	arrayList->length++;
}

void printArrayList(ArrayList *arrayList) {
	for(int i=0; i < arrayList->length; i++) {
		printf("Array[%d]=", i);
		for(int j=0; j<arrayList->data[i].length; j++) {
			printf("%d ", arrayList->data[i].data[j]);
		}
		putchar('\n');
	}
}