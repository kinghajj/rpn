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
void RPN_operatorAdd(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, a + b);
}

void RPN_operatorSubtract(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, a - b);
}

void RPN_operatorMultiply(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, a * b);
}

void RPN_operatorDivide(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, a / b);
}

void RPN_operatorPower(RPNStack *stack)
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

void RPN_operatorSqrt(RPNStack *stack)
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

void RPN_operatorEquals(RPNStack *stack)
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
void RPN_operatorModulo(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, (int)a % (int)b);
}

void RPN_operatorXor(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, (int)a ^ (int)b);
}

void RPN_operatorAnd(RPNStack *stack)
{
	RPNValue a, b;
	if(!RPN_canOperate(stack, 2)) return;
	b = RPN_pop(stack);
	a = RPN_pop(stack);
	RPN_push(stack, (int)a & (int)b);
}

void RPN_operatorOr(RPNStack *stack)
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
	RPN_dprintf("creating operator table %x", operators);
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
	RPN_dprintf("creating operator %x", operator);
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
	free(operator->op);
	free(operator);
	RPN_dprintf("freeing operator %x", operator);
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
	RPN_dprintf("removing operator %x from table %x", operator, operators);
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
	{
		RPN_removeOperator(operators, operator);
	}

	free(operators);
	RPN_dprintf("freeing operator table %x", operators);
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
	RPN_dprintf("adding operator %x to table %x", operator, operators);
	return true;
}

#ifndef DOXYGEN_SKIP

// this is the normal evaluation mode. an operator is only called once when it
// is received.
void RPN_normalMode(RPNCalculator *calculator, char *op)
{
	RPNOperator *operator;
	RPNVariable *var;
	char *name;
	bool everExecutedOp = false;
	bool everExecutedCmd = false;

	// try to find an operator
	HASH_FIND_STR( calculator->operators->table, op, operator );
	if(operator)
	{
		operator->func(calculator->stack);
		everExecutedOp = true;
	}

	// no? try to execute a command, then.
	if(!everExecutedOp)
		everExecutedCmd = RPN_executeCommand(calculator, op);

	// still no? then set this to a variable.
	if(!everExecutedOp && !everExecutedCmd)
	{
		// copy variable name
		name = strdup(op);

		// find variable with this name
		var = RPN_findVariable(calculator->variables, name);
		if(var) // THIS IS SPARTAAA!!!
		{
			// push it's value to the stack, free the name.
			RPN_push(calculator->stack, var->value);
			free(name);
		}
		else
		{
			// add a new variable to the variables table.
			RPN_addVariable(calculator->variables, name,
				RPN_peek(calculator->stack));
		}
	}
}

#endif // DOXYGEN_SKIP

//! Finds and executes and operator.
/**
 * @param calculator The calculator.
 * @param op The string representation of the operator.
 */
void RPN_executeOperator(RPNCalculator *calculator, char *op)
{
	if(!calculator)
		RPN_error("tried to execute an operator on a NULL table.");
	if(!op) return;

	RPN_normalMode(calculator, op);

	return;
}

//! Returns an operator table with default operators.
/**
 * @return The default operator table.
 */
RPNOperators *RPN_defaultOperators()
{
	RPNOperators *operators = RPN_newOperators();

	RPN_addOperator(operators, strdup("+"),    RPN_operatorAdd);
	RPN_addOperator(operators, strdup("-"),    RPN_operatorSubtract);
	RPN_addOperator(operators, strdup("*"),    RPN_operatorMultiply);
	RPN_addOperator(operators, strdup("/"),    RPN_operatorDivide);
	RPN_addOperator(operators, strdup("**"),   RPN_operatorPower);
	RPN_addOperator(operators, strdup("sqrt"), RPN_operatorSqrt);
	RPN_addOperator(operators, strdup("="),    RPN_operatorEquals);

	RPN_addOperator(operators, strdup("%"), RPN_operatorModulo);
	RPN_addOperator(operators, strdup("^"), RPN_operatorXor);
	RPN_addOperator(operators, strdup("&"), RPN_operatorAnd);
	RPN_addOperator(operators, strdup("|"), RPN_operatorOr);

	return operators;
}
