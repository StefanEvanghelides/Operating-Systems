#ifndef LIST_H
#define LIST_H

typedef enum TokenType {
	String,
	Symbol
} TokenType;

typedef struct ListNode *List;

typedef struct ListNode {
	TokenType type;
	char *data;
	List next;
} ListNode;



/* Prototypes. */

char *substring(char* string, int start, int end);
int isShellSymbol(char c);
List newNode(char* input, int *ip);
void printList(List li);
void freeList(List li);

#endif