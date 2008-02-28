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
 * The RPN token structure. (tokens.c)                                         *
 ******************************************************************************/

#ifndef RPN_TOKENS_H
#define RPN_TOKENS_H

// When a token array is first allocated, it can hold up to 10 items before it
// is reallocated. At the end, it is reallocated to hold only what it needs.
#define RPN_TOKENS_ALLOC_SIZE 10

//! Holds an array of tokens and the number of items in that array.
struct RPNTokens
{
	//! A pointer to an array of pointers to tokens.
	char **tokens;
	//! The number of items in the array.
	size_t size;
	//! How large the array is in memory.
	size_t alloc_size;
	//! Current position in the tokens array, for iterating.
	size_t pos;
};

RPNTokens *RPN_newTokens();
void RPN_addToken(RPNTokens *tokens, char *token);
RPNTokens *RPN_splitString(char *s);
void RPN_freeTokens(RPNTokens *tokens);

#endif // RPN_TOKENS_H
