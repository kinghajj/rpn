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
static bool isNumber(char *s)
{
	size_t i = 0, len = strlen(s);
	bool isnum = true;

	if(len && s[0] == '-')
		i = 1;

	for(; isnum && i < len; ++i)
		if(!isdigit(s[i]) && s[i] != '.') isnum = false;

	// caveat: if length is one and that's just a dot, then it's NOT a number.
	if(isnum && len == 1 && *s == '.')
		isnum = false;

	return isnum;
}

//! Evaluates a token on the calculator.
/**
 * @param calculator The calculator.
 * @param tok The token.
 */
static void evalToken(RPNCalculator *calculator, char *tok)
{
	RPNStack *stack = RPN_currentStack(calculator);
	RPNVariable *var;
	bool executed;

	// try to execute an operator; if that fails, then try to execute a command
	executed = RPN_executeOperator(calculator, tok) ||
	           RPN_executeCommand(calculator, tok);

	// no? then treat this as a variable name.
	if(!executed)
	{
		// find variable with this name
		var = RPN_findVariable(calculator->variables, tok);

		// if found,
		if(var) 
			// push it's value to the stack
			RPN_push(stack, var->value);
		else
			// add a new variable to the variable table, whose value is that of
			// the topmost item in the stack.
			RPN_addVariable(calculator->variables, strdup(tok),
			                RPN_peek(stack));
	}
}

//! Evaluates a string on a calculator.
/**
 * @param s The string to evaluate.
 * @param calculator The calculator on which to evaluate.
 * @return The topmost item of the calculator's stack.
 */
RPNValue RPN_eval(char *s, RPNCalculator *calculator)
{
	/* these are just used as shorthands to make the code more readable. */
	RPNTokens *tokens;
	char *token;

	/* split the string. */
	tokens = calculator->tokens = RPN_splitString(s);

	/* go through the tokens. */
	for(tokens->pos = 0; tokens->pos < tokens->size; ++tokens->pos)
	{
		token = tokens->tokens[tokens->pos];
		/* push numeric tokens to the stack. */
		if(isNumber(token))
			RPN_push(RPN_currentStack(calculator), RPN_strtod(token, NULL));
		/* delegate other tokens to evalToken(). */
		else
			evalToken(calculator, token);
	}

	/* cleanup. */
	RPN_freeTokens(tokens);
	calculator->tokens = NULL;

	if(calculator->needs_newline) {
		RPN_printf("\n");
		calculator->needs_newline = false;
	}

	return RPN_peek(RPN_currentStack(calculator));
}
