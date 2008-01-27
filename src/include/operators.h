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
 * The RPN operator table. (operators.c)                                       *
 ******************************************************************************/

#ifndef RPN_OPERATORS_H
#define RPN_OPERATORS_H

//! Operator callback type.
typedef void (*RPNOperatorFunc)(RPNStack *stack);

//! Holds an operator.
struct RPNOperator {
	//! The string representation of the operator.
	char *op;
	//! The function that performs the operator.
	RPNOperatorFunc func;
	//! A uthash handle to make this hashable.
	UT_hash_handle hh;
};

//! Holds a hash table of operators.
struct RPNOperators {
	//! The operator table.
	RPNOperator *table;
};

bool RPN_addOperator(RPNOperators *operators, char *op, RPNOperatorFunc func);
void RPN_freeOperators(RPNOperators *operators);
bool RPN_executeOperator(RPNCalculator *calculator, char *op);
RPNOperators *RPN_defaultOperators();

#endif // RPN_OPERATORS_H
