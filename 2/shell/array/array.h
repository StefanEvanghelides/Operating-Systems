#ifndef ARRAY_H
#define ARRAY_H

/* Structure representing the Array. */
typedef struct Array{
	int length;
	int maxSize;
	char *data;
} Array;

typedef struct ArrayList{
	int length;
	int maxSize;
	Array *data;
} ArrayList;

/* Prototypes. */
void initArray(Array *array);
void initArrayList(ArrayList *arrayList);
void doubleSize(Array *array);
void doubleSizeList(ArrayList *arrayList);
void addElement(Array *array, char element);
void addElementList(ArrayList *arrayList, Array element);
char **getArrayListData(ArrayList list);
void printArrayList(ArrayList list);

#endif
 
