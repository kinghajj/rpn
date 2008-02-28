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
 * error.c - an error-handling function; it just prints the error and exits,   *
 * complete with easter egg.                                                   *
 ******************************************************************************/

#include "rpn.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#ifndef DOXYGEN_SKIP

// returns a number from 1 to 50.
int lotto()
{
	return (rand() % 49) + 1;
}

// prints a lotto number then waits one second.
void printLotto()
{
	printf("%i ", lotto());
	fflush(stdout);
	sleep(1);
}

#endif // DOXYGEN_SKIP

//! Prints and error to stderr, then exits.
/**
 * @param msg The message to print.
 */
void RPN_error(char *msg)
{
	// boring stuff
	srand(time(NULL));
	fprintf(stderr, "error: %s\n", msg);

	// give the user some lotto numbers!
	printf("your lucky numbers: ");
	fflush(stdout);
	sleep(1);
	printLotto();
	printLotto();
	printLotto();
	printLotto();
	printLotto();
	fputc('\n', stdout);

	// ok, exit
	exit(-1);
}

//! Prints formatted output to stdout.
/**
 * This is used because some systems, such as the PlayStation Portable, require
 * special attention when printing. In the PSP sources, this function is defined
 * differently than in the normal sources, but the rest of the code doesn't know
 * that because they both have the same signature. Use this function to print
 * instead of printf() to ensure that your message is seen on all systems.
 *
 * @param fmt The formamming string.
 */
int RPN_printf(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	return vprintf(fmt, args);
}

#ifdef RPN_DEBUG
//! Only prints debug information if compiled to do so. If not, then the macro
//! RPN_dprintf is set to nothing, so this function isn't needed, and the
//! strings passed to it are not included in the executable, so there is no harm
//! in liberally using this function :)
int RPN_dprintff(char *func, char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	printf("%s: ", func);
	vprintf(fmt, args);
	printf("\n");
	return 0;
}
#endif // RPN_DEBUG
