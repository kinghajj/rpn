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
 * The RPN calculator structure. (calculator.c)                                *
 ******************************************************************************/

#ifndef RPN_CALCULATOR_H
#define RPN_CALCULATOR_H

//! The calculator should run normally.
#define RPN_STATUS_CONTINUE 1
//! The calculator should exit without error.
#define RPN_STATUS_EXIT     0

//! An all-in one structure needed by many functions.
/**
 * This structure contains a stack, variable table, operator table, command
 * table, and tokens. This structure is needed by many functions.
 */
struct RPNCalculator
{
	//! The calculator's available commands.
	RPNCommands *commands;
	//! The stack history.
	RPNHistory *history;
	//! The calculator's available operators.
	RPNOperators *operators;
	//! The calculator's stack.
	RPNStack *stack;
	//! The calculator's tokens.
	RPNTokens *tokens;
	//! The calcalator's variables.
	RPNVariables *variables;
	//! Holds the status of the calculator.
	int status;
};

RPNCalculator *RPN_newCalculator();
void RPN_freeCalculator(RPNCalculator *calculator);

#endif // RPN_CALCULATOR_H
