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
bool RPN_isNumber(char *s)
{
	size_t i, len = strlen(s);

	for(i = 0; i < len; i++)
		if(!isdigit(s[i]) && s[i] != '.') return false;
	
	return true;
}

//! Evaluates a token on the calculator.
/**
 * @param calculator The calculator.
 * @param tok The token.
 */
void RPN_evalToken(RPNCalculator *calculator, char *tok)
{
	RPNVariable *var;
	char *name;
	bool everExecutedOp;
	bool everExecutedCmd;

	// try to execute an operator
	everExecutedOp = RPN_executeOperator(calculator, tok);

	// no? try to execute a command, then.
	if(!everExecutedOp)
		everExecutedCmd = RPN_executeCommand(calculator, tok);

	// still no? then treat this as a variable name.
	if(!everExecutedOp && !everExecutedCmd)
	{
		// copy variable name
		name = strdup(tok);

		// find variable with this name
		var = RPN_findVariable(calculator->variables, name);
		if(var) 
		{
			// push it's value to the stack, free the name.
			RPN_push(calculator->stack, var->value);
			RPN_free(name);
		}
		// add a new variable to the variables table.
		else
		{
			RPN_addVariable(calculator->variables, name,
				RPN_peek(calculator->stack));
		}
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
	RPNTokens *tokens;

	calculator->tokens = RPN_splitString(s);
	tokens = calculator->tokens;

	for(tokens->pos = 0;
	    tokens->pos < tokens->size;
	    tokens->pos++)
	{
		if(RPN_isNumber(tokens->tokens[tokens->pos]))
			RPN_push(calculator->stack,
				RPN_strtod(tokens->tokens[tokens->pos], NULL));
		else
			RPN_evalToken(calculator, tokens->tokens[tokens->pos]);
	}

	RPN_freeTokens(tokens);
	calculator->tokens = NULL;

	return RPN_peek(calculator->stack);
}
