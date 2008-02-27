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
 * arguments.c -- processes argument options.                                  *
 ******************************************************************************/

#include "rpn.h"
#include <string.h>

#ifndef DOXYGEN_SKIP

bool RPN_argumentVersion(RPNCalculator *calculator, char **args)
{
	RPN_printf("%i.%i.%i.%i",
	           __RPN_MAJOR__,
	           __RPN_MINOR__,
	           __RPN_REVIS__,
	           __RPN_BUILD__);

	calculator->status = RPN_STATUS_EXIT;
	return false;
}

bool RPN_argumentHelp(RPNCalculator *calculator, char **args)
{
	RPN_printHelp();
	return false;
}

bool RPN_argumentExec(RPNCalculator *calculator, char **args)
{
	RPN_eval(args[0], calculator);
	RPN_printf(RPN_VALUE_LONG_FORMAT, RPN_peek(calculator->stack));

	calculator->status = RPN_STATUS_EXIT;
	return true;
}

RPNArgument RPN_arguments[] =
{
	{"-v", "--version", 0, RPN_argumentVersion},
	{"-h", "--help",    0, RPN_argumentHelp},
	{"-e", "--exec",    1, RPN_argumentExec},
	{0},
};

// tests if an argument is "null" or not.
inline bool RPN_argumentNotNull(RPNArgument *arg)
{
	if(arg->short_name && arg->long_name && arg->func)
		return true;
	return false;
}

RPNArgument *RPN_findArgument(char *name)
{
	int i;
	RPNArgument *argument;

	for(i = 0, argument = &RPN_arguments[i];
	    RPN_argumentNotNull(argument);
	    i++, argument = &RPN_arguments[i])
	{
		// check if the wanted name matches the short or long names.
		if(!strcmp(argument->short_name, name) ||
		   !strcmp(argument->long_name, name))
			return argument;
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
		argument = RPN_findArgument(argv[i]);
		if(argument && argc - i >= argument->nargs)
			if(!argument->func(calculator, &argv[i+1])) cont = 0;
	}
}
