#ifndef ARRAY_H
#define ARRAY_H

/* Structure representing the Array. */
typedef struct {
	int length;
	int maxSize;
	char **data;
} Array;

/* Prototypes. */
void initArray(Array *array, int size);
void doubleSize(Array *array);
void addElement(Array *array, char* element);
void freeArray(Array array);

#endif
 
