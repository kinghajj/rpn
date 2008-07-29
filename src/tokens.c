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
 *                                                                             *
 * this doesn't use strtok(), which would work, because I personally dislike   *
 * it. however, the gettok() function is equivalent.                           *
 ******************************************************************************/

#include <ctype.h>
#include "rpn.h"
#include <stdlib.h>
#include <string.h>

#ifndef DOXYGEN_SKIP

// returns true if c occurs in delims.
static inline bool is_delim(char c, char *delims)
{
	bool is = false;

	if(delims)
		while(*delims && !(is = (*delims++ == c)));

	return is;
}

#endif // DOXYGEN_SKIP

/**
 * @param start  A pointer to a string; keeps track of current position in
 *               original string.
 * @param delims Characters that will deliminate tokens.
 * @return A pointer to the first found token, or NULL if none found.
 *
 * gettok() works by taking in a pointer to a string, searching through that
 * string for deliminators, then updating that string so that on the next call,
 * it will return the next token.
 *
 * Like strtok(), gettok() modifies the original string by inserting NULs where
 * it finds deliminators.
 *
 * @code
 * void foo(char *str)
 * {
 *     char *start = str, *tok;
 *
 *     while((tok = gettok(&start, " ")))
 *         processToken(tok);
 * }
 * @endcode
 */
char *gettok(char **start, char *delims)
{
	char *search, *token;

	if(start && *start && **start && delims) {
		// start searching from the start, and assume the token will be here.
		search = token = *start;

		// find first delim.
		while(*search && !is_delim(*search, delims))
			search++;

		// nullify all consecutive delims.
		while(*search && is_delim(*search, delims))
			*search++ = '\0';

		// the next token is right after the end of the delims.
		*start = search;
	}
	else token = NULL;

	// if token is NULL, let it pass. if token is not NULL but points to a '\0',
	// then the token hasn't really been found, so recurse to find it.
	return (!token || *token) ? token : gettok(start, delims);
}

/**
 * Creates a new tokens array with the default initial size.
 *
 * @returns The new tokens array structure.
 */
static RPNTokens *newTokens()
{
	RPNTokens *tokens = new(RPNTokens);

	if(!tokens)
		RPN_error("could not allocate memory for tokens");

	tokens->alloc_size = RPN_TOKENS_ALLOC_SIZE;
	tokens->tokens = RPN_malloc(tokens->alloc_size * sizeof(char**));

	if(!tokens->tokens)
		RPN_error("could not allocate memory for tokens");

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
static void addToken(RPNTokens *tokens, char *token)
{
	if(!tokens)
		RPN_error("attempted to add token to a NULL token array.");
	if(!token)
		RPN_error("attempted to add a NULL token to token array.");

	// Check the tokens array for resizing.
	if(tokens->size >= tokens->alloc_size)
	{
		RPN_dprintf("tokens->size: %u", tokens->size);
		RPN_dprintf("tokens->alloc_size: %u", tokens->alloc_size);
		RPN_dprintf("tokens->tokens: %p", tokens->tokens);
		RPN_dprintf("token: \"%s\" (%u)", token, strlen(token));

		tokens->alloc_size += RPN_TOKENS_ALLOC_SIZE;
		tokens->tokens = realloc(tokens->tokens,
		                         tokens->alloc_size * sizeof(char**));
		if(!tokens->tokens)
			RPN_error("could not resize tokens");
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
	char *start = str, *tok;
	RPNTokens *tokens;

	if(!str)
		RPN_error("tried to split a null string");

	tokens = newTokens();

	while((tok = gettok(&start, " \t\n")))
		addToken(tokens, tok);

	// don't try to resize the tokens to null.
	if(tokens->size)
		tokens->tokens = realloc(tokens->tokens, tokens->size * sizeof(char**));
	if(!tokens->tokens)
		RPN_error("could not resize tokens");

	return tokens;
}

/**
 * Frees tokens.
 *
 * @param tokens The tokens to free.
 */
void RPN_freeTokens(RPNTokens *tokens)
{
	if(!tokens || !tokens->tokens)
		RPN_error("tried to free a null tokens structure");

	RPN_free(tokens->tokens);
	RPN_free(tokens);
}
