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
 * The RPN stack. Very important for calculation. (stack.c)                    *
 ******************************************************************************/

#ifndef RPN_STACK_H
#define RPN_STACK_H

//! An item in the stack.
struct RPNNode {
	//! The value of the item.
	RPNValue value;
	//! A pointer to the next item.
	struct RPNNode *next;
};

//! The stack.
struct RPNStack {
	//! The first item in the stack.
	RPNNode *first;
	//! The number of items in the stack.
	size_t len;
	//! A pointer to the next stack in the history.
	RPNStack *next;
};

RPNStack *RPN_newStack(RPNStack *next);
RPNNode *RPN_newNode(RPNValue value, RPNNode *next);
bool RPN_push(RPNStack *stack, RPNValue value);
RPNValue RPN_pop(RPNStack *stack);
RPNValue RPN_peek(RPNStack *stack);
void RPN_swap(RPNStack *stack);
void RPN_freeStack(RPNStack *stack);
bool RPN_canOperate(RPNStack *stack, unsigned int nargs);
void RPN_printStack(RPNStack *stack);
void RPN_printStackDetailed(RPNStack *stack);

#endif // RPN_STACK_H
