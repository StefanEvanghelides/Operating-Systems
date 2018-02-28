#ifndef ARRAY_H
#define ARRAY_H

/* Structure representing the Array. */
typedef struct Array{
	int length;
	int maxSize;
	char *data;
} Array;

typedef struct Array2D{
	int length;
	int maxSize;
	char **data;
} Array2D;

typedef struct ArrayList{
	int length;
	int maxSize;
	Array *data;
} ArrayList;

/* Prototypes. */
void initArray(Array *array);
void initArray2D(Array2D *array2d, int rows, int cols);
void initArrayList(ArrayList *arrayList);
void doubleSize(Array *array);
void doubleSize2D(Array2D *array2d);
void doubleSizeList(ArrayList *arrayList);
void addElement(Array *array, char element);
void addElement2D(Array2D *array2d, char* element);
void addElementList(ArrayList *arrayList, Array element);
void printArrayList(ArrayList *arrayList);

#endif
 
