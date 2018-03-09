#ifndef PARSER_H
#define PARSER_H

/* Prototypes. */
int isShellSymbol(char c);
List parseInput(char *input);
int acceptSymbol(List *tl, char *c);
int acceptString(List *tl);
int acceptCommand(List *tl);
int acceptNextCommand(List *tl);
int acceptInput(List *tl);

#endif
 
