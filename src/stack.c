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
	// this is the hidden node, always lurking at the bottom...
	stack->first = RPN_newNode(0.0, NULL);
	RPN_dprintf("creating stack %x", stack);
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
	node->next = next;
	RPN_dprintf("creating node at %x to %x", node, next);
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
	RPN_dprintf("pushed to stack");
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
	if(!stack)
		RPN_error("tried to pop from a NULL stack.");
	if(!stack->first->next) return 0;
	// get node to pop
	RPNNode *popped = stack->first;
	RPNValue value = popped->value;
	// re-wire stack
	stack->first = popped->next;
	// free node
	free(popped);
	RPN_dprintf("poping %x from stack", popped);
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
	return stack->first->value;
}

/**
 * Frees a stack and all of its nodes.
 *
 * @param stack The stack to free.
 */
void RPN_free(RPNStack *stack)
{
	RPNNode *node, *temp;

	// free all nodes
	for(node = stack->first; node;)
	{
		temp = node;
		node = node->next;
		free(temp);
		RPN_dprintf("freeing node %x", temp);
	}

	// free the stack
	free(stack);
	RPN_dprintf("freeing stack %x", stack);
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
	unsigned int nnodes;
	RPNNode *node;

	if(!stack)
		RPN_error("tried to probe a NULL stack.");

	for(nnodes = 0, node = stack->first; node; node = node->next, nnodes++);

	// can only operate if there are three nodes in the stack
	// (the third node is never used, but is the hidden bottom of the stack.)
	return (nnodes-1) >= nargs ? true : false;
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
	for(node = stack->first; node->next; node = node->next)
	{
#ifdef RPN_LONG_DOUBLE
		RPN_printf("%Lg, ", node->value);
#elif  RPN_DOUBLE
		RPN_printf("%g, ", node->value);
#endif
	}
	RPN_printf("]\n");
}

/**
 * Prints a stack in more detail.
 *
 * @param stack The stack.
 */
void RPN_printDetailed(RPNStack *stack)
{
	RPNNode *node;

	RPN_printf("[ ");
	for(node = stack->first; node->next; node = node->next)
	{
#ifdef RPN_LONG_DOUBLE
		RPN_dprintf("printing node %x", node);
		RPN_printf("%Lf, ", node->value);
#elif  RPN_DOUBLE
		RPN_printf("%f, ", node->value);
#endif
	}
	RPN_printf("]\n");
}
