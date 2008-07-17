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
 * stack.c - implements the value stack.                                       *
 ******************************************************************************/

#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Creates a new, empty stack.
 *
 * @return The new, empty stack.
 */
RPNStack *RPN_newStack()
{
	RPNStack *stack = new(RPNStack);
	if(!stack)
		RPN_error("could not allocate memory for the stack.");
	stack->first = NULL;
	stack->len   = 0;
	RPN_dprintf("allocated stack %p", stack);
	return stack;
}

/**
 * Creates a new node.
 * @param value The value of the node.
 *
 * @param next The next node in the stack.
 * @return The new node.
 */
RPNNode *RPN_newNode(RPNValue value, RPNNode *next)
{
	RPNNode *node = new(RPNNode);
	if(!node)
		RPN_error("could not allocate memory for a stack node.");
	node->value = value;
	node->next  = next;
	RPN_dprintf("allocated node at %p to %p", node, next);
	return node;
}

/**
 * Pushes a value to a stack.
 *
 * @param stack The stack.
 * @param value The value to push.
 * @return true if succeeds.
 */
bool RPN_push(RPNStack *stack, RPNValue value)
{
	if(!stack)
		RPN_error("tried to push to a NULL stack.");
	RPNNode *node = RPN_newNode(value, stack->first);
	stack->first = node;
	stack->len++;
	RPN_dprintf("pushed " RPN_VALUE_SHORT_FORMAT  " to stack", value);
	return true;
}

/**
 * Pops a value from a stack.
 *
 * @param stack The stack to pop.
 * @return The popped value.
 */
RPNValue RPN_pop(RPNStack *stack)
{
	RPNNode *popped;
	RPNValue value = 0;

	if(!stack)
		RPN_error("tried to pop from a NULL stack.");
	if(RPN_canOperate(stack, 1))
	{
		// get node to pop
		popped = stack->first;
		value = popped->value;
		// re-wire stack
		stack->first = popped->next;
		// free node
		RPN_free(popped);
		stack->len--;
		RPN_dprintf("poped %x from stack", popped);
	}

	return value;
}

/**
 * Returns the topmost item of a stack.
 *
 * @param stack The stack.
 * @return The topmost item of the stack.
 */
RPNValue RPN_peek(RPNStack *stack)
{
	RPNValue value = 0;

	if(!stack)
		RPN_error("attempted to peek a NULL stack.");

	if(RPN_canOperate(stack, 1))
		value = stack->first->value;

	return value;
}

/**
 * Frees a stack and all of its nodes.
 *
 * @param stack The stack to free.
 */
void RPN_freeStack(RPNStack *stack)
{
	RPNNode *node, *next;

	// free all nodes
	for(node = stack->first; node; node = next)
	{
		next = node->next;
		RPN_free(node);
		RPN_dprintf("freed node %p", node);
	}

	// free the stack
	RPN_free(stack);
	RPN_dprintf("freed stack %p", stack);
}

/**
 * Tests if the stack has at least a certain number of items.
 *
 * @param stack The stack to test.
 * @param nargs The number of required items.
 * @return true or false.
 */
bool RPN_canOperate(RPNStack *stack, unsigned int nargs)
{
	if(!stack)
		RPN_error("tried to probe a NULL stack.");

	// can only operate if there are enough items in the stack
	return stack->len >= nargs ? true : false;
}

/**
 * Prints a stack.
 *
 * @param stack The stack.
 */
void RPN_printStack(RPNStack *stack)
{
	RPNNode *node;

	RPN_printf("[ ");
	for(node = stack->first; node; node = node->next)
		RPN_printf(RPN_VALUE_SHORT_FORMAT ", ", node->value);
	RPN_printf("]\n");
}

/**
 * Prints a stack in more detail.
 *
 * @param stack The stack.
 */
void RPN_printStackDetailed(RPNStack *stack)
{
	RPNNode *node;

	RPN_printf("[ ");
	for(node = stack->first; node; node = node->next)
		RPN_printf(RPN_VALUE_LONG_FORMAT ", ", node->value);
	RPN_printf("]\n");
}
