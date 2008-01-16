#include <ctype.h>
#include "rpn.h"
#include <stdlib.h>
#include <string.h>

#ifndef DOXYGEN_SKIP

size_t RPN_findNextToken(size_t cur_pos, char *s, size_t len)
{
	size_t pos = cur_pos;

	if(!s || !len) return 0;

	while(pos < len && isspace(s[pos])) pos++;

	return pos;
}

size_t RPN_findTokenEnd(size_t cur_pos, char *s, size_t len)
{
	size_t pos = cur_pos;

	if(!s || !len) return 0;

	while(pos < len && !isspace(s[pos])) pos++;

	return pos;
}

char *RPN_getNextToken(char *str, size_t len, size_t *pos, size_t *end)
{
	size_t size;
	char *s;

	// find the size of the token
	*pos = RPN_findNextToken(*pos, str, len);
	*end = RPN_findTokenEnd(*pos, str, len);
	size = *end - *pos + 1;

	// allocate space for it
	s = malloc(size);

	// copy it from the source string
	memcpy(s, &str[*pos], size);

	// make sure that it's null-terminated
	s[size - 1] = 0;

	// update the position to be after the end of the token
	*pos = *end + 1;

	return s;
}

#endif // DOXYGEN_SKIP

/**
 * Creates a new tokens array with the default initial size.
 *
 * @returns The new tokens array structure.
 */
RPNTokens *RPN_newTokens()
{
	RPNTokens *tokens;

	tokens = new(RPNTokens);
	if(!tokens) RPN_error("could not allocate memory for tokens");
	tokens->alloc_size = RPN_TOKENS_ALLOC_SIZE;
	tokens->tokens = malloc(tokens->alloc_size * sizeof(char**));
	if(!tokens->tokens) RPN_error("could not allocate memory for tokens");
	tokens->size = 0;
	return tokens;
}

/**
 * Adds a token to a tokens array structure. Automatically resizes the array if
 * needed.
 *
 * @param tokens The array to which to add the token.
 * @param token The token to add.
 */
void RPN_addToken(RPNTokens *tokens, char *token)
{
	if(!tokens)
		RPN_error("attempted to add token to a NULL token array.");
	if(!token)
		RPN_error("attempted to add a NULL token to token array.");
	
	// Check the tokens array.
	if(tokens->size >= tokens->alloc_size)
	{
		tokens->alloc_size += RPN_TOKENS_ALLOC_SIZE;
		tokens->tokens = realloc(tokens->tokens,
			tokens->alloc_size * sizeof(char**));
		if(!tokens->tokens) RPN_error("could not resize tokens");
	}

	// store token in token structure
	tokens->tokens[tokens->size++] = token;
}

/**
 * Splits a string into tokens using whitespace as the delimeter.
 *
 * @param str The string to split.
 * @return A RPNTokens structure with the tokens.
 */
RPNTokens *RPN_splitString(char *str)
{
	size_t len = strlen(str), pos = 0, end;
	RPNTokens *tokens;

	if(!str) RPN_error("tried to split a null string");

	tokens = RPN_newTokens();

	// Add tokens until we run out.
	while(pos < len)
		RPN_addToken(tokens, RPN_getNextToken(str, len, &pos, &end));

	// resize array
	tokens->tokens = realloc(tokens->tokens, tokens->size * sizeof(char**));
	if(!tokens->tokens) RPN_error("could not resize tokens");

	return tokens;
}

/**
 * Frees tokens.
 *
 * @param tokens The tokens to free.
 */
void RPN_freeTokens(RPNTokens *tokens)
{
	size_t i;

	if(!tokens || !tokens->tokens)
		RPN_error("tried to free a null tokens structure");

	for(i = 0; i < tokens->size; i++)
		if(!tokens->tokens[i]) RPN_error("tried to free a null token");
		else free(tokens->tokens[i]);
	
	free(tokens->tokens);
	free(tokens);
}
