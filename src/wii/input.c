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

/* input.c - a simple input module for the Wii.
 *
 * I tried using libosk to have a keyboard, but that looks like it will be too
 * much work to get working right, so instead I'm trying this.
 *
 * This input system is simple and intuitive: press up to search up for the
 * letter/character you want, down to search down, and left-right to move around
 * the buffer. When done, press A to evaluate the string.
 */

#include "rpn.h"
#include <stdint.h>
#include <stdio.h>

#ifndef DOXYGEN_SKIP

#define BUFFER_SIZE 64

static char characters[] =
	" 0123456789"
	"+-/*=%^&|"
	"abcdefghijklmnopqrstuvwxyz"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

typedef struct {
	int pos;
	char buf[BUFFER_SIZE];
} InputBuffer;

static void init_buffer(InputBuffer *input)
{
	int i;

	input->pos = 0;
	for(i = 0; i < BUFFER_SIZE; ++i)
		input->buf[i] = ' ';
}

static void print_buffer(InputBuffer *input)
{
	int i;

	for(i = 0; i < BUFFER_SIZE; ++i)
		printf("%c", input->buf[i]);
}

static void erase_buffer()
{
	int i;

	for(i = 0; i < BUFFER_SIZE; ++i)
		printf("\b");
}

static inline int find_character(char c)
{
	int i = 0;
	char *chars = characters;
	bool found = false;

	for(i = 0; !found && characters[i]; ++i)
		if(chars[i] == c)
			found = true;

	return found ? i - 1 : -1;
}

static inline char get_next_character(char c)
{
	char ret = 0;
	int character_pos;

	character_pos = find_character(c) + 1;

	if(character_pos)
		ret = characters[character_pos];

	return ret;
}

static inline char get_prev_character(char c)
{
	char ret = 0;
	int character_pos;

	character_pos = find_character(c) - 1;

	if(character_pos >= 0)
		ret = characters[character_pos];

	return ret;
}

static inline void update_buffer(InputBuffer *input, char c)
{
	if(c) {
		input->buf[input->pos] = c;
		erase_buffer();
		print_buffer(input);
	}
}

#endif /* DOXYGEN_SKIP */

/**
 * Gets a string from user input from a wiimote.
 * @return The entered string.
 */
char *RPNWii_GetInput()
{
	static InputBuffer input;
	uint32_t pressed = 0;

	init_buffer(&input);
	print_buffer(&input);

	while(pressed != WPAD_BUTTON_A) {
		// wait for user inputs.
		WPAD_ScanPads();

		// get inputs for the first player.
		pressed = WPAD_ButtonsDown(WPAD_CHAN_0);

		// handle various button presses.
		switch(pressed) {
			case WPAD_BUTTON_UP:
				update_buffer(&input, get_next_character(input.buf[input.pos]));
				break;
			case WPAD_BUTTON_DOWN:
				update_buffer(&input, get_prev_character(input.buf[input.pos]));
				break;
			case WPAD_BUTTON_LEFT:
				if(input.pos) input.pos--;
				break;
			case WPAD_BUTTON_RIGHT:
				if(input.pos < BUFFER_SIZE - 1) input.pos++;
				break;
		}

		// wait for video.
		VIDEO_WaitVSync();
	}

	printf("\n");
	return input.buf;
}
