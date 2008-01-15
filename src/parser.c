/*******************************************************************************
 * Reverse Polish Notation calculator.                                         *
 * Copyright (c) 2007, Samuel Fredrickson <kinghajj@gmail.com>                 *
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
 * parser.c - parses RPN expressions.                                          *
 ******************************************************************************/

#include <ctype.h>
#include "rpn.h"
#include <stdlib.h>
#include <string.h>

//! Tests if a string can be converted into a number.
/**
 * @param s The string to test.
 * @return true or false.
 */
bool RPN_isNumber(char *s)
{
	size_t i, len = strlen(s);

	for(i = 0; i < len; i++)
		if(!isdigit(s[i]) && s[i] != '.') return false;
	
	return true;
}

//! Evaluates a string on a calculator.
/**
 * @param s The string to evaluate.
 * @param calculator The calculator on which to evaluate.
 * @return The topmost item of the calculator's stack.
 */
RPNValue RPN_eval(char *s, RPNCalculator *calculator)
{
	size_t i;

	calculator->tokens = RPN_splitString(s);

	for(i = 0; i < calculator->tokens->size; i++)
	{
		if(RPN_isNumber(calculator->tokens->tokens[i]))
#ifdef RPN_DOUBLE
			RPN_push(calculator->stack,
				strtod(calculator->tokens->tokens[i], NULL));
#elif RPN_LONG_DOUBLE
			RPN_push(calculator->stack,
				strtold(calculator->tokens->tokens[i], NULL));
#endif
		else
			RPN_executeOperator(calculator, calculator->tokens->tokens[i]);
	}

	RPN_freeTokens(calculator->tokens);
	calculator->tokens = NULL;

	return RPN_peek(calculator->stack);
}
