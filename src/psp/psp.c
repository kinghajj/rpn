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

/* psp.c - the PSP front-end.
 */

#include "rpn.h"

// Tell the PSP about the program.
PSP_MODULE_INFO("PSPRPN", 0, 1, 1);

int main()
{
	RPNCalculator *calculator;
	RPNValue peek = 0;
	char *input;

	// Setup the PSP screen, callbacks, etc.
	RPNPSP_Setup();

	// Start Program.
	kprintf("PSPRPN v. %i.%i.%i.%i\n",
		__RPN_MAJOR__,
		__RPN_MINOR__,
		__RPN_REVIS__,
		__RPN_BUILD__);

#ifdef __GNUC__
	kprintf("GCC %i.%i.%i on %s at %s\n",
		__GNUC__,
		__GNUC_MINOR__,
		__GNUC_PATCHLEVEL__,
		__DATE__, __TIME__);
#endif

	calculator = RPN_newCalculator();

	// Input loop.
	while(calculator->status == RPN_STATUS_CONTINUE)
	{
		kprintf("[%g]> ", peek);
		input = RPNPSP_GetString();
		if(input)
			peek = RPN_eval(input, calculator);
	}

	sceKernelExitGame();

	return 0;
}
