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
 * functions.c -- processes user-defined functions.                            *
 ******************************************************************************/

#include "rpn.h"

//! Creates a new function table.
/**
 * @return The new function table.
 */
RPNFunctions *RPN_newFunctions()
{
	RPNFunctions *functions = new(RPNFunctions);
	if(!functions)
		RPN_error("could not allocate memory for function table.");
	// uthash requires that an empty hash be set to NULL.
	functions->table = NULL;
	RPN_dprintf("created function table %x", functions);
	return functions;
}

//! Creates a new function.
/* @param name The name of the function.
 * @param arg_names A pointer to an array of strings of the argument names.
 * @param arg_size The size of arg_names.
 * @param code The code of the function.
 */
RPNFunction *RPN_newFunction(char *name, char **arg_names, size_t arg_size,
                             char *code)
{
	RPNFunction *function = new(RPNFunction);
	if(!function)
		RPN_error("could not allocate memory for function.");
	
	if(!name)
		RPN_error("attempted to give new function a NULL name.");
	else if(!arg_names)
		RPN_error("attempted to give new function NULL argument names.");
	else if(!code)
		RPN_error("attempted to give new function NULL code.");
	
	function->name = name;
	function->arg_names = arg_names;
	function->arg_size = arg_size;
	function->code = code;

	return function;
}

//! Adds a new function to a function table.
/* @param functions The function table.
 * @param name The name of the function.
 * @param arg_names A pointer to an array of strings of the argument names.
 * @param arg_size The size of arg_names.
 * @param code The code of the function.
 */
void RPN_addFunction(RPNFunctions *functions, char *name, char **arg_names,
                     size_t arg_size, char *code)
{
	RPNFunction *function = RPN_newFunction(name, arg_names, arg_size, code);
	if(!functions)
		RPN_error("tried to add function to NULL function table.");
	HASH_ADD_KEYPTR( hh, functions->table, name, strlen(name), function );
	RPN_dprintf("added function %x to table %x", function, functions);
}
