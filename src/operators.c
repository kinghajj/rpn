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
 * operators.c - implements the operators table.                               *
 ******************************************************************************/

#include <math.h>
#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * The RPN operator functions, in all their glory.                             *
 ******************************************************************************/

#ifndef DOXYGEN_SKIP

// Basic math operators.
static RPNValue operatorAdd(RPNValue a, RPNValue b)
{
	return a + b;
}

static RPNValue operatorSubtract(RPNValue a, RPNValue b)
{
	return a - b;
}

static RPNValue operatorMultiply(RPNValue a, RPNValue b)
{
	return a * b;
}

static RPNValue operatorDivide(RPNValue a, RPNValue b)
{
	return a / b;
}

static RPNValue operatorPower(RPNValue a, RPNValue b)
{
#ifdef RPN_LONG_DOUBLE
	return powl(a, b);
#elif  RPN_DOUBLE
	return pow(a, b);
#endif
}

static RPNValue operatorEquals(RPNValue a, RPNValue b)
{
	return a == b;
}

// Modulo and bitwise operators. These are converted to integers, so they lose
// data.
static RPNValue operatorModulo(RPNValue a, RPNValue b)
{
	return (int)a % (int)b;
}

static RPNValue operatorXor(RPNValue a, RPNValue b)
{
	return (int)a ^ (int)b;
}

static RPNValue operatorAnd(RPNValue a, RPNValue b)
{
	return (int)a & (int)b;
}

static RPNValue operatorOr(RPNValue a, RPNValue b)
{
	return (int)a | (int)b;
}

#endif // DOXYGEN_SKIP

/*******************************************************************************
 * The RPN operator table. Allows for simple but powerful extendability.       *
 ******************************************************************************/

//! Creates a new operator table.
/**
 * @return The new operator table.
 */
RPNOperators *RPN_newOperators()
{
	RPNOperators *operators = new(RPNOperators);
	if(!operators)
		RPN_error("could not allocate memory for operator table.");
	// uthash requires that an empty hash be set to NULL.
	operators->table = NULL;
	RPN_dprintf("allocated operator table %p", operators);
	return operators;
}

//! Creates a new operator.
/**
 * @param op The string representation of the operator.
 * @param func The function that performs the operator.
 * @return The new operator.
 */
RPNOperator *RPN_newOperator(char *op, RPNOperatorFunc func)
{
	RPNOperator *operator = new(RPNOperator);
	if(!operator)
		RPN_error("could not allocate memory for operator.");
	operator->op = op;
	operator->func = func;
	RPN_dprintf("allocated operator %p", operator);
	return operator;
}

//! Frees an operator and its string representation.
/**
 * @param operator The operator to free.
 */
void RPN_freeOperator(RPNOperator *operator)
{
	if(!operator || !operator->op)
		RPN_error("attempted to free a NULL operator.");
	RPN_free(operator->op);
	RPN_free(operator);
	RPN_dprintf("freed operator %p", operator);
}

//! Removes an operator from an operator table, then frees it.
/**
 * @param operators The operator table.
 * @param operator The operator.
 */
void RPN_removeOperator(RPNOperators *operators, RPNOperator *operator)
{
	if(!operators)
		RPN_error("attempted to remove an operator from a NULL operator"
		          " table.");
	if(!operator)
		RPN_error("attempted to remove a NULL operator.");
	HASH_DEL( operators->table, operator );
	RPN_freeOperator(operator);
	RPN_dprintf("removed operator %p from table %p", operator, operators);
}

//! Frees an operator table and all of its operators.
/**
 * @param operators The operators table.
 */
void RPN_freeOperators(RPNOperators *operators)
{
	RPNOperator *operator, *next;

	// go through all operators and remove/free them.
	for(operator = operators->table; operator != NULL; operator = next) {
		next = operator->hh.next;
		RPN_removeOperator(operators, operator);
	}

	RPN_free(operators);
	RPN_dprintf("freed operator table %p", operators);
}

//! Adds an operator to an operator table.
/**
 * @param operators The operator table.
 * @param op The string representation of the operator.
 * @param func The function that performs the operator.
 * @return true if succeeds.
 */
bool RPN_addOperator(RPNOperators *operators, char *op, RPNOperatorFunc func)
{
	RPNOperator *operator = RPN_newOperator(op, func);
	if(!operators)
		RPN_error("tried to add operator to NULL operator table.");
	HASH_ADD_KEYPTR( hh, operators->table, op, strlen(op), operator );
	RPN_dprintf("added operator %p to table %p", operator, operators);
	return true;
}

//! Finds an operator in an operator table by name.
/**
 * @param operators The operator table.
 * @param op The name of the operator to find.
 * @return The operator if found, else NULL.
 */
RPNOperator *RPN_findOperator(RPNOperators *operators, char *op)
{
	RPNOperator *operator;

	HASH_FIND_STR( operators->table, op, operator );
	
	return operator;
}

//! Finds and executes and operator.
/**
 * @param calculator The calculator.
 * @param op The string representation of the operator.
 */
bool RPN_executeOperator(RPNCalculator *calculator, char *op)
{
	RPNOperator *operator;
	RPNValue a, b;
	bool executed = false;

	if(!calculator)
		RPN_error("tried to execute an operator on a NULL table.");

	if(op)
	{
		// find the operator.
		operator = RPN_findOperator(calculator->operators, op);
		// execute it, return.
		if(operator && RPN_canOperate(calculator->stack, 2))
		{
			b = RPN_pop(calculator->stack);
			a = RPN_pop(calculator->stack);
			RPN_push(calculator->stack, operator->func(a, b));
			executed = true;
		}
	}

	return executed;
}

//! Returns an operator table with default operators.
/**
 * @return The default operator table.
 */
RPNOperators *RPN_defaultOperators()
{
	RPNOperators *operators = RPN_newOperators();

	RPN_addOperator(operators, strdup("+"),    operatorAdd);
	RPN_addOperator(operators, strdup("-"),    operatorSubtract);
	RPN_addOperator(operators, strdup("*"),    operatorMultiply);
	RPN_addOperator(operators, strdup("/"),    operatorDivide);
	RPN_addOperator(operators, strdup("**"),   operatorPower);
	RPN_addOperator(operators, strdup("="),    operatorEquals);

	RPN_addOperator(operators, strdup("%"), operatorModulo);
	RPN_addOperator(operators, strdup("^"), operatorXor);
	RPN_addOperator(operators, strdup("&"), operatorAnd);
	RPN_addOperator(operators, strdup("|"), operatorOr);

	return operators;
}
