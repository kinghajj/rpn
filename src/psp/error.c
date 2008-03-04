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
 * error.c - an error-handling function; it just prints the error and exits.   *
 * there's also some output code.                                              *
 ******************************************************************************/

#include "rpn.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//! Prints an error to the screen, then exits. This is much simpler than the
//! normal RPN_error function.
/**
 * @param msg The message to print.
 */
void RPN_error(char *msg)
{
	kprintf("\nerror: %s\n\nauto-exiting in 10 seconds...", msg);
	sleep(10);
	sceKernelExitGame();
}

#ifndef DOXYGEN_SKIP
#define RPNPSP_OUTBUF_SIZE 1024
char RPNPSP_output_buffer[RPNPSP_OUTBUF_SIZE];

static void clearOutputBuffer()
{
	int i;

	for(i = 0; i < RPNPSP_OUTBUF_SIZE; i++)
	{
		RPNPSP_output_buffer[i] = 0;
	}
}
#endif // DOXYGEN_SKIP

//! Prints to the screen.
/**
 * The PSP, as far as I can tell, cannot use normal printf()-family functions.
 * So, this uses vsprintf to print to a buffer first. This could potentially be
 * dangerous, so the buffer is fairly large (1024 bytes) to prevent errors. As
 * long as you don't print anything longer than 1024 characters long, everything
 * should be fine.
 *
 * @param fmt The formatting string.
 */
int RPN_printf(char *fmt, ...)
{
	va_list args;

	// Always make sure to clear the buffer!
	clearOutputBuffer();

	va_start(args, fmt);
	vsprintf(RPNPSP_output_buffer, fmt, args);
	kprintf("%s", RPNPSP_output_buffer);
	return 0;
}

#ifndef DOXYGEN_SKIP
#ifdef RPN_DEBUG
int RPN_dprintff(char *func, char *fmt, ...)
{
	va_list args;

	// Always make sure to clear the buffer!
	RPNPSP_clearOutputBuffer();

	va_start(args, fmt);
	vsprintf(RPNPSP_output_buffer, fmt, args);
	kprintf("%s: %s\n", func, RPNPSP_output_buffer);
	return 0;
}
#endif
#endif // DOXYGEN_SKIP
