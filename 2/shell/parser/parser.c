#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../array/array.h"
#include "parser.h"

/* Checks if we reached the end of a token, i.e. a delimiter. */
int endOfToken(Array input, int pos) {
	return (pos < input.length && 
			input.data[pos] != '<' &&
			input.data[pos] != '>' &&
			input.data[pos] != '&' &&
			input.data[pos] != '|' ); 
}

// TODO
Array parseToken(Array input, int *i, int type) {
	Array token;
	initArray(&token);

	int pos = *i;

	while(pos < input.length && input.data[pos] == ' ') pos++; //removes spaces

	while(!endOfToken(input, pos)){
		addElement(&token, input.data[pos]);
		pos++;
	}

	i = &pos;
	return token;
}

// TODO
ArrayList parseRedirects(Array input, int *i) {
	ArrayList tokens;
	initArrayList(&tokens);

	// if(i < input.length) {
	// 	Array commandToken, inputToken, outputToken;
	// 	initArray(&commanToken);
	// 	initArray(&inputToken);
	// 	initArray(&outputToken);

	// 	commandToken = parseToken(input, i, '<');
	// 	inputToken = parse


		// while(i < input.length && input.data[i] == ' ') i++; //removes spaces
		
		// while(i < input.length && input.data[i] != '<'){
		// 	addElement(&currentToken, input.data[i]);
		// 	i++;
		// }
		// addElementList(&tokens, currentToken);
		// i++;

		// while(i < input.length && input.data[i] != '>'){
		// 	addElement(&currentToken, input.data[i]);
		// 	i++;
		// }
		// addElementList(&tokens, currentToken);
		// i++;
	// }

	return tokens;
}

// TODO
ArrayList parsePipes() {
	ArrayList tokens;
	initArrayList(&tokens);

	return tokens;
}

// TODO
ArrayList parseBackgroundOperator() {
	ArrayList tokens;
	initArrayList(&tokens);

	return tokens;
}

/* Parses the input. */
ArrayList parseInput(Array input) {
	ArrayList tokens;
	initArrayList(&tokens);

	int i=0;
	while(i < input.length) {
		Array currentToken;
		initArray(&currentToken);

		while(i < input.length && input.data[i] == ' ') i++; //removes spaces
		
		while(i < input.length && input.data[i] != '&') {
			addElement(&currentToken, input.data[i]);
			i++;
		}
		addElementList(&tokens, currentToken);
		i++;
	}

	return tokens;
}
