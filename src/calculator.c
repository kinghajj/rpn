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
 * calculator.c - an all-in-one structure that has a stack, operators,         *
 * commands and variables.                                                     *
 ******************************************************************************/

#include "rpn.h"
#include <stdlib.h>

//! Creates a new calculator that has a stack, default commands, default
//! operators, default variables, and an empty list of tokens.
/**
 * It is recomended that you use this function to create a calculator rather
 * than creating a the stack and tables on your own, unless you have some
 * special need.
 *
 * @return A new RPNCalculator structure.
 */
RPNCalculator *RPN_newCalculator()
{
	RPNCalculator *calculator = new(RPNCalculator);

	if(!calculator)
		RPN_error("could not allocate memory for calculator.");

	// Allocate the members.
	RPN_dprintf("allocating calculator members");
	calculator->commands  = RPN_defaultCommands();
	calculator->operators = RPN_defaultOperators();
	calculator->stack     = RPN_newStack();
	calculator->status    = RPN_STATUS_CONTINUE;
	calculator->tokens    = NULL;
	calculator->variables = RPN_defaultVariables();

	return calculator;
}

//! Frees a calculator and all of its members, such as the stack, commands,
//! operators, variables, and the token list.
/**
 * @param calculator A pointer to the calculator to free.
 */
void RPN_freeCalculator(RPNCalculator *calculator)
{
	if(!calculator)
		RPN_error("tried to free a NULL calculator.");

	// Free every member, then the calculator itself.
	RPN_dprintf("freeing calculator stack");
	RPN_freeStack(calculator->stack);
	RPN_dprintf("freeing calculator variables");
	RPN_freeVariables(calculator->variables);
	RPN_dprintf("freeing calculator operators");
	RPN_freeOperators(calculator->operators);
	RPN_dprintf("freeing calculator commands");
	RPN_freeCommands(calculator->commands);
	RPN_dprintf("freeing calculator");
	free(calculator);
}
