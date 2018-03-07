#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "list.h"

/* Extracts the substring of a string. */
char *substring(char* string, int start, int end) {
  char *sub = calloc((end - start + 1), sizeof(char));
 
  for(int k = start; k < end; k++) {
    sub[k - start] = string[k];
  }
 
  return sub;
}

/* Creates a new list node. */
List newNode(char* input, int *ip) { 
  List node = malloc(sizeof(struct ListNode));
  assert(node != NULL);  

  int length = strlen(input);

  if(isShellSymbol(input[*ip])){ /* It is a symbol. */
    node->data = malloc(2*sizeof(char));
    node->data[0] = input[*ip];
    node->data[1] = '\0';
    node->type = Symbol;
    (*ip)++;

  } else { /* It is not a symbol. Parse the string. */
    int j = *ip;
    while(j < length && !isspace(input[j]) && !isShellSymbol(input[j])) j++;
    
    node->data = substring(input, *ip, j);
    printf("node->data = %s\n",node->data);
    node->type = String;
    *ip = j;
  }

  node->next = NULL;

  return node;
}

/* Prints the List on the standard output. */
void printList(List li) {
  printf("List=[");
  while (li != NULL) {
    printf("%s ",li->data);
    li = li->next;
  }

  printf("]\n");
}

/* Free the memory. */
void freeList(List li) {
  if (li == NULL) {
    return;
  }
  if(li->data != NULL) free(li->data);
  freeList(li->next);
  free(li);
}