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
	uint32_t pressed = 0;
	wsp::GameWindow gwd;
	OnScreenKeyboard *osk;

	RPNWii_Setup();
	gwd.InitVideo();

	printf("Hello, world!\nWelcome to RPN!\nMore to come soon ;)\n");

	osk = new OnScreenKeyboard(&gwd, "/config/key_config.xml");

	while(!wpad_button_home_pressed(pressed)) {
		// wait for user inputs.
		WPAD_ScanPads();
		// get inputs for the first player.
		pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
		if(pressed) {
			printf("Button code: %x\n", pressed);
			buzz_wpad(WPAD_CHAN_0, 1000);
		}
		// wait for video.
		VIDEO_WaitVSync();
	}

	printf("Exiting...\n");

	return 0;
}
