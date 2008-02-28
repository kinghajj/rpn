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
 * The RPN variable table. (variables.c)                                       *
 ******************************************************************************/

#ifndef RPN_VARIABLES_H
#define RPN_VARIABLES_H

//! A variable.
struct RPNVariable {
	//! The stringe representation of the variable.
	char *name;
	//! The value of the variable.
	RPNValue value;
	//! A uhtash handle to make this hashable.
	UT_hash_handle hh;
};

//! Holds a hash table of variables.
struct RPNVariables {
	//! The variable table.
	RPNVariable *table;
};

RPNVariables *RPN_newVariables();
RPNVariable *RPN_newVariable(char *name, RPNValue value);
bool RPN_addVariable(RPNVariables *variables, char *name, RPNValue value);
RPNVariable *RPN_findVariable(RPNVariables *variables, char *name);
void RPN_freeVariables(RPNVariables *variables);
void RPN_printVariables(RPNVariables *variables);
void RPN_printVariablesDetailed(RPNVariables *variables);
RPNVariables *RPN_defaultVariables();

#endif // RPN_VARIABLES_H
