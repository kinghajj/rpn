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

/* wii.c - the Wii front-end.
 */

#include "rpn.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <OnScreenKeyboard.h>
#include <wiisprite.h>

inline static void buzz_wpad(int32_t chan, int useconds)
{
	// start rumbling.
	WPAD_Rumble(chan, 1);
	// wait.
	usleep(useconds);
	// stop.
	WPAD_Rumble(chan, 0);
}

inline static bool wpad_button_home_pressed(uint32_t pressed)
{
	return pressed & WPAD_BUTTON_HOME ||
	       pressed & WPAD_CLASSIC_BUTTON_HOME;
}

int main(int argc, char **argv)
{
	RPNCalculator *calculator;
	char *input;

	RPNWii_Setup();

	calculator = RPN_newCalculator();

	// Input loop.
	while(calculator->status == RPN_STATUS_CONTINUE)
	{
		printf("\n\n\n\n\n[%g]> ", RPN_peek(calculator->stack));
		input = RPNWii_GetInput();
		if(input)
			RPN_eval(input, calculator);
	}

}
