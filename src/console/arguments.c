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
 * arguments.c -- processes argument options.                                  *
 ******************************************************************************/

#include "rpn.h"
#include <string.h>

//! Callback for arguments. If a callback returns true, then the program will
//! continue to look for other arguments; if false, then the program exits.
typedef bool (*RPNArgumentFunc)(RPNCalculator *calculator, char **args);

//! Holds an argument option.
struct RPNArgument
{
	//! The short name of the option, like "-v".
	char *short_name;
	//! The long name of the option, like "--version".
	char *long_name;
	//! The number of arguments the option wants.
	size_t nargs;
	//! The function that performs the option.
	RPNArgumentFunc func;
};

typedef struct RPNArgument RPNArgument;

#ifndef DOXYGEN_SKIP

static bool argumentVersion(RPNCalculator *calculator, char **args)
{
	RPN_printf("%i.%i.%i.%i",
	           __RPN_MAJOR__,
	           __RPN_MINOR__,
	           __RPN_REVIS__,
	           __RPN_BUILD__);
#ifdef RPN_DEBUG
	RPN_printf("-debug");
#endif
#ifdef __GNUC__
	RPN_printf(" (GCC %i.%i.%i on %s at %s)", __GNUC__,
	                                          __GNUC_MINOR__,
	                                          __GNUC_PATCHLEVEL__,
	                                          __DATE__, __TIME__);
#endif

	calculator->status = RPN_STATUS_EXIT;
	return false;
}

static bool argumentHelp(RPNCalculator *calculator, char **args)
{
	RPN_printHelp();
	return false;
}

static bool argumentExec(RPNCalculator *calculator, char **args)
{
	RPN_eval(args[0], calculator);
	RPN_printf(RPN_VALUE_LONG_FORMAT, RPN_peek(calculator->stack));

	calculator->status = RPN_STATUS_EXIT;
	return true;
}

// tests if an argument is "null" or not.
static inline bool argumentNotNull(RPNArgument *arg)
{
	if(arg->short_name && arg->long_name && arg->func)
		return true;
	return false;
}

static RPNArgument *findArgument(char *name)
{
	int i;
	static RPNArgument arguments[] =
	{
	//   short form  long form     #args  callback
		{"-v",       "--version",  0,     argumentVersion},
		{"-h",       "--help",     0,     argumentHelp},
		{"-e",       "--exec",     1,     argumentExec},
		{0},
	};

	for(i = 0; argumentNotNull(&arguments[i]); i++)
	{
		// check if the wanted name matches the short or long names.
		if(!strcmp(arguments[i].short_name, name) ||
		   !strcmp(arguments[i].long_name, name))
			return &arguments[i];
	}

	return NULL;
}

#endif // DOXYGEN_SKIP

//! Goes through arguments and executes any valid options it sees.
/**
 * @param calculator The calculator on which options will operate.
 * @param argc The number of arguments.
 * @param argv The arguments.
 */
void RPN_processArguments(RPNCalculator *calculator, int argc, char *argv[])
{
	int i;
	int cont = 1;
	RPNArgument *argument;

	/* Go through the arguments:
	 *
	 *   -Find if the argument is defined.
	 *   -If found and there are enough arguments, executed and stop iterating
	 *    through the arguments.
	 */
	for(i = 0; i < argc && cont; i++)
	{
		argument = findArgument(argv[i]);
		if(argument && argc - i >= argument->nargs)
			if(!argument->func(calculator, &argv[i+1])) cont = 0;
	}
}
