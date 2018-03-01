#ifndef PARSER_H
#define PARSER_H

/* Prototypes. */
int endOfToken(Array input, int pos);
Array parseToken(Array input, int *i, int type);
ArrayList parseRedirects(Array input, int *i);
ArrayList parsePipes();
ArrayList parseBackgroundOperator();
ArrayList parseInput(Array input);


#endif
 
