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
 * help.c - a simple help system for the front-end. this is very loose from    *
 * the rest of the program; you could easily take this and use it yourself.    *
 ******************************************************************************/

#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * Help-related variables and functions.
 ******************************************************************************/

#ifndef DOXYGEN_SKIP

/* These are the various classes of CommandHelpItems.
 */
static RPNCommandHelpItem options_help[] =
{
	{"-v or --version", "Display version."},
	{NULL},
};

static RPNCommandHelpItem operators_help[] = {
	{"+, -, *, /, **, sqrt, =", "Normal math operators."},
	{"%, ^, &, |",              "Modulo and bitwise operators."},
	{NULL},
};

static RPNCommandHelpItem commands_help[] = {
	{"dup",  "Pushes topmost value to the stack."},
	{"pop",  "Removes the topmost value of the stack."},
	{"ps",   "Prints the stack. Doesn't print anything if empty."},
	{"psd",  "Prints the stack in detail. Doesn't print anything if"
	         "\n\t\tempty."},
	{"psv",  "Prints the variable table. Doesn't print anything if"
	         "\n\t\tempty."},
	{"psvd", "Prints the variable table in detail. Doesn't print"
	         "\n\t\tanything if empty."},
	{"x",    "Exits the program."},
	{NULL},
};

/* This prints an array of CommandHelpItems, like those found just above.
 */
void RPN_printCommandHelpItemArray(char *title, RPNCommandHelpItem *items)
{
	RPNCommandHelpItem item;
	int i;

	printf("%s:\n", title);

	// print every help item.
	for(i = 0, item = items[i]; item.command; i++, item = items[i])
    {
		RPN_printf("\t%s\n\t\t%s\n", item.command, item.help);
	}
}

void RPN_printOptionsHelp()
{
	RPN_printCommandHelpItemArray("Options", options_help);
}

void RPN_printOperatorsHelp()
{
	RPN_printCommandHelpItemArray("Operators", operators_help);
}

void RPN_printCommandsHelp()
{
	RPN_printCommandHelpItemArray("Commands", commands_help);
}

void RPN_printHelp()
{
	RPN_printf("rpn v. %i.%i.%i.%i -- a Reverse Polish Notation calculator\n",
	           __RPN_MAJOR__,
	           __RPN_MINOR__,
	           __RPN_REVIS__,
	           __RPN_BUILD__);
	RPN_printf("By Sam Fredrickson <kinghajj@gmail.com>\n\n");

	RPN_printOptionsHelp();
	RPN_printOperatorsHelp();
	RPN_printCommandsHelp();
}

#endif // DOXYGEN_SKIP
