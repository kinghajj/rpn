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
 * commands.c - implements the commands table.                                 *
 ******************************************************************************/

#include "rpn.h"
#include <stdlib.h>

/*******************************************************************************
 * The RPN command functions, in all their glory.                              *
 ******************************************************************************/

// Don't look at these, Doxygen.
#ifndef DOXYGEN_SKIP

void RPN_commandPrint(RPNCalculator *calculator, char **args)
{
	RPN_printStack(calculator->stack);
}

void RPN_commandPrintDetailed(RPNCalculator *calculator, char **args)
{
	RPN_printStackDetailed(calculator->stack);
}

void RPN_commandPrintVariables(RPNCalculator *calculator, char **args)
{
	RPN_printVariables(calculator->variables);
}

void RPN_commandPrintVariablesDetailed(RPNCalculator *calculator, char **args)
{
	RPN_printVariablesDetailed(calculator->variables);
}

void RPN_commandPrintHelp(RPNCalculator *calculator, char **args)
{
	RPN_printHelp();
}

void RPN_commandExit(RPNCalculator *calculator, char **args)
{
	calculator->status = RPN_STATUS_EXIT;
}

void RPN_commandPop(RPNCalculator *calculator, char **args)
{
	RPN_pop(calculator->stack);
}

void RPN_commandDup(RPNCalculator *calculator, char **args)
{
	if(RPN_canOperate(calculator->stack, 1))
		RPN_push(calculator->stack, RPN_peek(calculator->stack));
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
	RPN_dprintf("created commands table %x", commands);
	return commands;
}

//! Creates a new command, but does not insert it into a table.
/**
 * @param cmd The string representation of the command.
 * @param func The function that performs the command.
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
	RPN_dprintf("created command %x", command);
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
	free(command->cmd);
	free(command);
	RPN_dprintf("freed command %x", command);
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
	RPN_dprintf("removed command %x from table %x", command, commands);
}

//! Frees a command table and all if its commands.
/**
 * @param commands The command table to free.
 */
void RPN_freeCommands(RPNCommands *commands)
{
	RPNCommand *command;

	// go through every command and remove/free it.
	for(command = commands->table; command != NULL; command = command->hh.next)
		RPN_removeCommand(commands, command);

	free(commands);
	RPN_dprintf("freed command table %x", commands);
}

//! Adds a command to a command table.
/**
 * @param commands The command table.
 * @param cmd The string representation of a command.
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
	RPN_dprintf("added command %x to table %x", command, commands);
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

	HASH_FIND_STR( calculator->commands->table, cmd, command );
	if(!command) return false;
	if(tokens->size - tokens->pos + 1 >= command->nargs)
		command->func(calculator, &tokens->tokens[tokens->pos + 1]);
	return true;
}

//! Returns a command table with default commands.
/**
 * @return The default commands.
 */
RPNCommands *RPN_defaultCommands()
{
	RPNCommands *commands = RPN_newCommands();

	RPN_addCommand(commands, strdup("dup"),  0, RPN_commandDup);
	RPN_addCommand(commands, strdup("pop"),  0, RPN_commandPop);
	RPN_addCommand(commands, strdup("ps"),   0, RPN_commandPrint);
	RPN_addCommand(commands, strdup("psd"),  0, RPN_commandPrintDetailed);
	RPN_addCommand(commands, strdup("psv"),  0, RPN_commandPrintVariables);
	RPN_addCommand(commands, strdup("psvd"), 0,
		RPN_commandPrintVariablesDetailed);
	RPN_addCommand(commands, strdup("help"), 0, RPN_commandPrintHelp);
	RPN_addCommand(commands, strdup("x"),    0, RPN_commandExit);
	RPN_dprintf("created default command table");

	return commands;
}
