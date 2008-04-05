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
 * variables.c - implements the variables table.                               *
 ******************************************************************************/

#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//! Creates a new, empty variables table.
/**
 * @return The new, empty variables table.
 */
RPNVariables *RPN_newVariables()
{
	RPNVariables *variables = new(RPNVariables);
	if(!variables)
		RPN_error("could not allocate memory for a variables table.");
	// uthash.h requires that an empty table hash be set to NULL.
	variables->table = NULL;
	RPN_dprintf("created variable table %x", variables);
	return variables;
}

//! Creates a new variable.
/**
 * @param name The string representation of the variable.
 * @param value The value of the variable.
 * @return The new variable.
 */
RPNVariable *RPN_newVariable(char *name, RPNValue value)
{
	RPNVariable *var = new(RPNVariable);
	if(!var)
		RPN_error("could not allocate memory for a variable.");
	var->name = name;
	var->value = value;
	RPN_dprintf("created variable %x", var);
	return var;
}

//! Adds a variable to a variable table.
/**
 * @param variables The variable table.
 * @param name The string representation of the variable.
 * @param value The value of the variable.
 * @return true if succeeds.
 */
bool RPN_addVariable(RPNVariables *variables, char *name, RPNValue value)
{
	RPNVariable *var;

	if(!variables)
		RPN_error("tried to push variable to a NULL stack.");
	var = RPN_newVariable(name, value);
	// name of hash header, name of hash table, name of key, length of key,
	// structure to add
	HASH_ADD_KEYPTR( hh, variables->table, name, strlen(name), var );
	RPN_dprintf("added variable %x to table %x", var, variables);
	return true;
}

//! Finds a variable by its string representation.
/**
 * @param variables The variable table.
 * @param name The string representation of the variable.
 * @return The variable if found, else NULL.
 */
RPNVariable *RPN_findVariable(RPNVariables *variables, char *name)
{
	RPNVariable *var = NULL;
	if(!variables)
		RPN_error("tried to find variable in NULL stack.");
	if(!name)
		RPN_error("tried to find a variable with a NULL name.");
	HASH_FIND_STR( variables->table, name, var);
	return var;
}

//! Gets the value of a variable by its string representation.
/**
 * @param variables The variable table.
 * @param name The string representation of the variable.
 * @return The value if found, else 0.
 */
RPNValue RPN_getVariableValue(RPNVariables *variables, char *name)
{
	RPNVariable *var = RPN_findVariable(variables, name);
	RPNValue value = 0;

	if(!variables)
		RPN_error("tried to find variable in a NULL variable table.");
	if(var)
		value = var->value;

	return value;
}

//! Frees a variable and its string representation.
/**
 * @param variable The variable to free.
 */
void RPN_freeVariable(RPNVariable *variable)
{
	if(!variable || !variable->name)
		RPN_error("attempted to free a NULL variable.");
	RPN_free(variable->name);
	RPN_free(variable);
	RPN_dprintf("freed variable %x", variable);
}

//! Removes a variable from a variable table, then frees it.
/**
 * @param variables The variable table.
 * @param variable The variable to remove.
 */
void RPN_removeVariable(RPNVariables *variables, RPNVariable *variable)
{
	if(!variables)
		RPN_error("attempted to remove a variable from a NULL variable table.");
	if(!variable)
		RPN_error("attempted to remove a NULL variable.");
	HASH_DEL( variables->table, variable );
	RPN_freeVariable(variable);
	RPN_dprintf("removed variable %x from table %x", variable, variables);
}

//! Frees a variable table and all its variables.
/**
 * @param variables The variable table to free.
 */
void RPN_freeVariables(RPNVariables *variables)
{
	RPNVariable *variable;

	// go through every command and remove/free it.
	for(variable = variables->table;
	    variable != NULL;
	    variable = variable->hh.next)
		RPN_removeVariable(variables, variable);

	RPN_free(variable);
	RPN_dprintf("freed variable table %x", variables);
}

//! Prints the variables of a variable table.
/**
 * @param variables The variable table to print.
 */
void RPN_printVariables(RPNVariables *variables)
{
	RPNVariable *var;

	RPN_printf("[ ");
	for(var = variables->table; var != NULL; var = var->hh.next)
		RPN_printf("%s = " RPN_VALUE_SHORT_FORMAT ", ", var->name, var->value);
	RPN_printf("]\n");
}

//! Prints the variables of a variable table in more detail.
/**
 * @param variables The variable table to print.
 */
void RPN_printVariablesDetailed(RPNVariables *variables)
{
	RPNVariable *var;

	RPN_printf("[ ");
	for(var = variables->table; var != NULL; var = var->hh.next)
		RPN_printf("%s = " RPN_VALUE_LONG_FORMAT ", ", var->name, var->value);
	RPN_printf("]\n");
}

//! Returns a default variable table.
/**
 * @return The default variable table.
 */
RPNVariables *RPN_defaultVariables()
{
	RPNVariables *variables = RPN_newVariables();

	// Computer sizes.
	RPN_addVariable(variables, strdup("KB"),  1000);
	RPN_addVariable(variables, strdup("MB"),  1000 * 1000);
	RPN_addVariable(variables, strdup("GB"),  1000 * 1000 * 1000);
	RPN_addVariable(variables, strdup("KiB"), 1024);
	RPN_addVariable(variables, strdup("MiB"), 1024 * 1024);
	RPN_addVariable(variables, strdup("GiB"), 1024 * 1024 * 1024);

	// Mathematical numbers.
	RPN_addVariable(variables, strdup("PI"), 3.1415926535897932385);
	RPN_addVariable(variables, strdup("E"),  2.72828182845904523536);

	return variables;
}
