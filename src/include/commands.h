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
 * The RPN command table. (commands.c)                                         *
 ******************************************************************************/

#ifndef RPN_COMMANDS_H
#define RPN_COMMANDS_H

//! Command callback type.
typedef void (*RPNCommandFunc)(RPNCalculator *calculator, char **args);

//! Holds a command.
struct RPNCommand {
	//! The string representation of the command.
	char *cmd;
	//! The function that performs the command.
	RPNCommandFunc func;
	//! The number of arguments the argument takes.
	size_t nargs;
	//! A uthash handle to make this hashable.
	UT_hash_handle hh;
};

//! Holds a hash table of commands.
struct RPNCommands {
	//! The command table.
	RPNCommand *table;
};

void RPN_freeCommands(RPNCommands *commands);
void RPN_addCommand(RPNCommands *commands, char *cmd, size_t nargs,
                    RPNCommandFunc func);
bool RPN_executeCommand(RPNCalculator *calculator, char *cmd);
RPNCommands *RPN_defaultCommands();

#endif // RPN_COMMANDS_H
