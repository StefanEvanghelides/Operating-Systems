#ifndef PARSER_H
#define PARSER_H

/* Prototypes. */
int isShellSymbol(char c);
List parseInput(char *input);
int acceptSymbol(List *tl, char *c);
int acceptString(List *tl);
int acceptRedirect(List *tl);
int acceptPipe(List *tl);
int acceptBackgroundProcess(List *tl);
int acceptInput(List *tl);

#endif
 
