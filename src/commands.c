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
 * commands.c - implements the commands table.                                 *
 ******************************************************************************/

#include <math.h>
#include "rpn.h"
#include <stdlib.h>

/*******************************************************************************
 * The RPN command functions, in all their glory.                              *
 ******************************************************************************/

// Don't look at these, Doxygen.
#ifndef DOXYGEN_SKIP

static void commandPrint(RPNCalculator *calculator, char **args)
{
	RPN_printStack(calculator->stack);
}

static void commandPrintDetailed(RPNCalculator *calculator, char **args)
{
	RPN_printStackDetailed(calculator->stack);
}

static void commandPrintVariables(RPNCalculator *calculator, char **args)
{
	RPN_printVariables(calculator->variables);
}

static void commandPrintVariablesDetailed(RPNCalculator *calculator,
                                          char **args)
{
	RPN_printVariablesDetailed(calculator->variables);
}

static void commandPrintHelp(RPNCalculator *calculator, char **args)
{
	RPN_printHelp();
}

static void commandExit(RPNCalculator *calculator, char **args)
{
	calculator->status = RPN_STATUS_EXIT;
}

static void commandPop(RPNCalculator *calculator, char **args)
{
	RPN_pop(calculator->stack);
}

static void commandDup(RPNCalculator *calculator, char **args)
{
	if(RPN_canOperate(calculator->stack, 1))
		RPN_push(calculator->stack, RPN_peek(calculator->stack));
}

static void commandSqrt(RPNCalculator *calculator, char **args)
{
	RPNValue a;
	if(!RPN_canOperate(calculator->stack, 1)) return;
	a = RPN_pop(calculator->stack);
#ifdef RPN_LONG_DOUBLE
	RPN_push(calculator->stack, sqrtl(a));
#elif RPN_DOUBLE
	RPN_push(calculator->stack, sqrt(a));
#endif
}

static void commandUnset(RPNCalculator *calculator, char **args)
{
	char *varname = args[0];
	RPNVariable *variable = RPN_findVariable(calculator->variables, varname);
	if(variable)
		RPN_removeVariable(calculator->variables, variable);
}

#endif // DOXYGEN_SKIP

/*******************************************************************************
 * The RPN command table. Allows for simple but powerful extendability.        *
 ******************************************************************************/

//! Creates a new, empty command hash table.
/**
 * @return A new, empty command hash table.
 */
RPNCommands *RPN_newCommands()
{
	RPNCommands *commands = new(RPNCommands);
	if(!commands)
		RPN_error("could not allocate memory for command table.");
	// uthash requires that an empty table be set to NULL.
	commands->table = NULL;
	RPN_dprintf("allocated commands table %p", commands);
	return commands;
}

//! Creates a new command, but does not insert it into a table.
/**
 * @param cmd   The string representation of the command.
 * @param nargs The number of arguments the command takes.
 * @param func  The function that performs the command.
 * @return A new command.
 */
RPNCommand *RPN_newCommand(char *cmd, size_t nargs, RPNCommandFunc func)
{
	RPNCommand *command = new(RPNCommand);
	if(!command)
		RPN_error("could not allocate memory for command.");
	command->cmd = cmd;
	command->nargs = nargs;
	command->func = func;
	RPN_dprintf("allocated command %p", command);
	return command;
}

//! Frees a command and its string representation.
/**
 * @param command The command to free.
 */
void RPN_freeCommand(RPNCommand *command)
{
	if(!command || !command->cmd)
		RPN_error("attempted to free a NULL command.");
	RPN_free(command->cmd);
	RPN_free(command);
	RPN_dprintf("freed command %p", command);
}

//! Removes a command from a command table, then frees it.
/**
 * @param commands The command table.
 * @param command The command to remove.
 */
void RPN_removeCommand(RPNCommands *commands, RPNCommand *command)
{
	if(!commands)
		RPN_error("attempted to remove a command from a NULL command table.");
	if(!command)
		RPN_error("attempted to remove a NULL command.");
	HASH_DEL( commands->table, command );
	RPN_freeCommand(command);
	RPN_dprintf("removed command %p from table %p", command, commands);
}

//! Frees a command table and all if its commands.
/**
 * @param commands The command table to free.
 */
void RPN_freeCommands(RPNCommands *commands)
{
	RPNCommand *command, *next;

	// go through every command and remove/free it.
	for(command = commands->table; command != NULL; command = next) {
		next = command->hh.next;
		RPN_removeCommand(commands, command);
	}

	RPN_free(commands);
	RPN_dprintf("freed command table %p", commands);
}

//! Adds a command to a command table.
/**
 * @param commands The command table.
 * @param cmd The string representation of a command.
 * @param nargs The number of arguments the command needs.
 * @param func The function that performs the command.
 * @return true if succeeds.
 */
void RPN_addCommand(RPNCommands *commands, char *cmd, size_t nargs,
                    RPNCommandFunc func)
{
	RPNCommand *command = RPN_newCommand(cmd, nargs, func);
	if(!commands)
		RPN_error("tried to add command to NULL command table.");
	HASH_ADD_KEYPTR( hh, commands->table, cmd, strlen(cmd), command );
	RPN_dprintf("added command %p to table %p", command, commands);
}

//! Finds a command based on its string representation.
/**
 * @param commands The commands table to search.
 * @param cmd The string representation of the command.
 * @return A pointer to the command.
 */
RPNCommand *RPN_findCommand(RPNCommands *commands, char *cmd)
{
	RPNCommand *command;
	HASH_FIND_STR( commands->table, cmd, command );
	return command;
}

//! Finds and executes a command based on its string representation.
/**
 * @param calculator The calculator on which to execute the command.
 * @param cmd The string representation of the command.
 * @return true if found and executed, false otherwise.
 */
bool RPN_executeCommand(RPNCalculator *calculator, char *cmd)
{
	RPNCommand *command;
	RPNTokens *tokens = calculator->tokens;
	bool executed = false;

	command = RPN_findCommand(calculator->commands, cmd);
	if(command)
	{
		// only execute if there are enough arguments.
		if(tokens->size - tokens->pos + 1 >= command->nargs)
			command->func(calculator, &tokens->tokens[tokens->pos + 1]);
		// skip the arguments
		tokens->pos += command->nargs;
		executed = true;
	}

	return executed;
}

//! Returns a command table with default commands.
/**
 * @return The default commands.
 */
RPNCommands *RPN_defaultCommands()
{
	RPNCommands *commands = RPN_newCommands();

	RPN_addCommand(commands, strdup("dup"),   0, commandDup);
	RPN_addCommand(commands, strdup("pop"),   0, commandPop);
	RPN_addCommand(commands, strdup("ps"),    0, commandPrint);
	RPN_addCommand(commands, strdup("psd"),   0, commandPrintDetailed);
	RPN_addCommand(commands, strdup("psv"),   0, commandPrintVariables);
	RPN_addCommand(commands, strdup("psvd"),  0, commandPrintVariablesDetailed);
	RPN_addCommand(commands, strdup("help"),  0, commandPrintHelp);
	RPN_addCommand(commands, strdup("x"),     0, commandExit);
	RPN_addCommand(commands, strdup("sqrt"),  0, commandSqrt);
	RPN_addCommand(commands, strdup("unset"), 1, commandUnset);
	RPN_dprintf("created default command table");

	return commands;
}
