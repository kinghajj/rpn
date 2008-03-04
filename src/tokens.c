/*******************************************************************************
 * Reverse Polish Notation calculator.                                         *
 * Copyright (c) 2007-2008, Samuel Fredrickson <kinghajj@gmail.com>            *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 *     * Redistributions of source code must retain the above copyright        *
 *       notice, this list of conditions and the following disclaimer.         *
 *     * Redistributions in binary form must reproduce the above copyright     *
 *       notice, this list of conditions and the following disclaimer in the   *
 *       documentation and/or other materials provided with the distribution.  *
 *                                                                             *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER ``AS IS'' AND ANY EXPRESS *
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED           *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE      *
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY        *
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES  *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR          *
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER  *
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT          *
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY   *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH *
 * DAMAGE.                                                                     *
 ******************************************************************************/

/*******************************************************************************
 * tokens.c -- splits a string into tokens.                                    *
 ******************************************************************************/

#include <ctype.h>
#include "rpn.h"
#include <stdlib.h>
#include <string.h>

#ifndef DOXYGEN_SKIP

char *RPN_NULL_TOKEN = "(NULL_TOKEN)";

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

	// no real token? then return NULL. handle it later.
	if(size == 1) return RPN_NULL_TOKEN;

	// allocate space for it
	s = RPN_malloc(size);

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
	tokens->tokens = RPN_malloc(tokens->alloc_size * sizeof(char**));
	if(!tokens->tokens) RPN_error("could not allocate memory for tokens");
	tokens->size = 0;
	tokens->pos = 0;
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
	if(token == RPN_NULL_TOKEN)
		return;

	// Check the tokens array.
	if(tokens->size >= tokens->alloc_size)
	{
		RPN_dprintf("tokens->size: %u", tokens->size);
		RPN_dprintf("tokens->alloc_size: %u", tokens->alloc_size);
		RPN_dprintf("tokens->tokens: %p", tokens->tokens);
		RPN_dprintf("token: \"%s\" (%u)", token, strlen(token));

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
	RPN_dprintf("tokens->size: %u", tokens->size);
	RPN_dprintf("tokens->tokens: %p", tokens->tokens);
	// don't try to resize the tokens to null.
	if(tokens->size)
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
		else RPN_free(tokens->tokens[i]);
	
	RPN_free(tokens->tokens);
	RPN_free(tokens);
}
