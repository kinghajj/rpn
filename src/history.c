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
 * history.c - implements the history stack.                                   *
 ******************************************************************************/

#include "rpn.h"
#include <stdio.h>

RPNHistory *RPN_newHistory()
{
	RPNHistory *history = new(RPNHistory);

	if(history)
		history->first = RPN_newStack(NULL);

	return history;
}

void RPN_freeHistory(RPNHistory *history)
{
	if(history) {
		while(history->first)
			RPN_popHistory(history);
		RPN_freeStack(history->first);
		RPN_free(history);
	}
}

void RPN_pushHistory(RPNHistory *history)
{
	RPNStack *copy;

	if(history && history->first) {
		copy = RPN_copyStack(history->first);
		copy->next = history->first;
		history->first = copy;
	}
}

void RPN_popHistory(RPNHistory *history)
{
	RPNStack *orig;

	if(history && history->first) {
		orig = history->first;
		history->first = history->first->next;
		RPN_freeStack(orig);
	}
}

void RPN_printHistory(RPNHistory *history)
{
	RPNStack *stack;

	RPN_printf("[");
	if(history)
		for(stack = history->first; stack; stack = stack->next) {
			RPN_printStack(stack);
			RPN_printf(", ");
		}
	RPN_printf("]");
}

void RPN_printHistoryDetailed(RPNHistory *history)
{
	RPNStack *stack;

	RPN_printf("[");
	if(history)
		for(stack = history->first; stack; stack = stack->next) {
			RPN_printStack(stack);
			RPN_printf(", ");
		}
	RPN_printf("]");
}
