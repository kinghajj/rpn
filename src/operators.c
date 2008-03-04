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
 * remember that math operators need to pop in reverse order because of the    *
 * way stacks work.                                                            *
 ******************************************************************************/

#ifndef DOXYGEN_SKIP

// Basic math operators.
static void operatorAdd(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, a + b);
}

static void operatorSubtract(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, a - b);
}

static void operatorMultiply(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, a * b);
}

static void operatorDivide(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, a / b);
}

static void operatorPower(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
#ifdef RPN_LONG_DOUBLE
	RPN_push(stack, powl(a, b));
#elif  RPN_DOUBLE
	RPN_push(stack, pow(a, b));
#endif
}

static void operatorSqrt(RPNStack *stack)
{
	RPNValue a;
	if(!RPN_canOperate(stack, 1)) return;
	a = RPN_pop(stack);
#ifdef RPN_LONG_DOUBLE
	RPN_push(stack, sqrtl(a));
#elif RPN_DOUBLE
	RPN_push(stack, sqrt(a));
#endif
}

static void operatorEquals(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack,2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	if(a == b)
		RPN_push(stack, (RPNValue)1);
	else
		RPN_push(stack, (RPNValue)0);
}

// Modulo and bitwise operators. These are converted to integers, so they lose
// data.
static void operatorModulo(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, (int)a % (int)b);
}

static void operatorXor(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, (int)a ^ (int)b);
}

static void operatorAnd(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, (int)a & (int)b);
}

static void operatorOr(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, (int)a | (int)b);
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
	RPN_dprintf("created operator table %x", operators);
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
	RPN_dprintf("created operator %x", operator);
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
	RPN_dprintf("freed operator %x", operator);
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
	RPN_dprintf("removed operator %x from table %x", operator, operators);
}

//! Frees an operator table and all of its operators.
/**
 * @param operators The operators table.
 */
void RPN_freeOperators(RPNOperators *operators)
{
	RPNOperator *operator;

	// go through all operators and remove/free them.
	for(operator = operators->table;
	    operator != NULL;
	    operator = operator->hh.next)
		RPN_removeOperator(operators, operator);

	RPN_free(operators);
	RPN_dprintf("freed operator table %x", operators);
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
	RPN_dprintf("added operator %x to table %x", operator, operators);
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

	if(!calculator)
		RPN_error("tried to execute an operator on a NULL table.");
	if(!op) return false;

	// find the operator.
	operator = RPN_findOperator(calculator->operators, op);
	// execute it, return.
	if(operator)
	{
		operator->func(calculator->stack);
		return true;
	}

	// not an operator?
	return false;
}
// THIS IS SPARTAAA!!!
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
	RPN_addOperator(operators, strdup("sqrt"), operatorSqrt);
	RPN_addOperator(operators, strdup("="),    operatorEquals);

	RPN_addOperator(operators, strdup("%"), operatorModulo);
	RPN_addOperator(operators, strdup("^"), operatorXor);
	RPN_addOperator(operators, strdup("&"), operatorAnd);
	RPN_addOperator(operators, strdup("|"), operatorOr);

	return operators;
}
