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
 * rpn.c - the front-end.                                                      *
 *                                                                             *
 * This is stack-based, so expressions can be as long as memory will allow.    *
 *   Operators can be as long as you want. You can add your own by writing an  *
 * operator function then calling RPN_addOperator().                           *
 *   I've commented this code to explain any 'magic' or otherwise cryptic      *
 * sections. I hope they will suffice.                                         *
 ******************************************************************************/

#include "rpn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * The main part of the program.
 ******************************************************************************/

#ifndef DOXYGEN_SKIP

void RPN_printVersion()
{
	printf("%i.%i.%i.%i\n",
		__RPN_MAJOR__,
		__RPN_MINOR__,
		__RPN_REVIS__,
		__RPN_BUILD__);
	exit(0);
}

int main(int argc, char *argv[])
{
	RPNCalculator *calculator = RPN_newCalculator();
	char input[1024];

	RPN_processArguments(calculator, argc, argv);

	while(calculator->status == RPN_STATUS_CONTINUE)
	{
#ifdef RPN_LONG_DOUBLE
		printf("[%Lg]> ", RPN_peek(calculator->stack));
#elif  RPN_DOUBLE
		printf("[%g]> ", RPN_peek(calculator->stack));
#endif
		fgets(input, 1024, stdin);
		if(feof(stdin)) break;
		RPN_eval(input, calculator);
	}

	fputc('\n', stdout);

	// cleanup.
	RPN_freeCalculator(calculator);

	return 0;
}

#endif // DOXYGEN_SKIP
