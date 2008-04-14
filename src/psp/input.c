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

/* input.c - input module.
 */

#include "rpn.h"

#ifndef DOXYGEN_SKIP

//! Holds information on how a certain button press on the PSP maps to
//! characters. The alternate form is accessed by pressing the right trigger.
struct RPNPSPKey
{
	int buttons;
	char chr;
	char alt_chr;
};
typedef struct RPNPSPKey RPNPSPKey;

// The master key map.
static RPNPSPKey KeyMap[] =
{
	// Numbers
	{PSP_CTRL_CROSS,                  '0', '0'},
	{PSP_CTRL_CROSS | PSP_CTRL_UP,    '1', '1'},
	{PSP_CTRL_CROSS | PSP_CTRL_RIGHT, '2', '2'},
	{PSP_CTRL_CROSS | PSP_CTRL_DOWN,  '3', '3'},
	{PSP_CTRL_CROSS | PSP_CTRL_LEFT,  '4', '4'},

	{PSP_CTRL_CIRCLE,                  '5', '5'},
	{PSP_CTRL_CIRCLE | PSP_CTRL_UP,    '6', '6'},
	{PSP_CTRL_CIRCLE | PSP_CTRL_RIGHT, '7', '7'},
	{PSP_CTRL_CIRCLE | PSP_CTRL_DOWN,  '8', '8'},
	{PSP_CTRL_CIRCLE | PSP_CTRL_LEFT,  '9', '9'},

	// Special characters
	{PSP_CTRL_SQUARE,                  '.', '='},
	{PSP_CTRL_SQUARE | PSP_CTRL_UP,    '+', '+'},
	{PSP_CTRL_SQUARE | PSP_CTRL_RIGHT, '-', '-'},
	{PSP_CTRL_SQUARE | PSP_CTRL_DOWN,  '*', '*'},
	{PSP_CTRL_SQUARE | PSP_CTRL_LEFT,  '/', '/'},

	{PSP_CTRL_TRIANGLE,                  ' ', ' '},
	{PSP_CTRL_TRIANGLE | PSP_CTRL_UP,    '%', '%'},
	{PSP_CTRL_TRIANGLE | PSP_CTRL_RIGHT, '^', '^'},
	{PSP_CTRL_TRIANGLE | PSP_CTRL_DOWN,  '&', '&'},
	{PSP_CTRL_TRIANGLE | PSP_CTRL_LEFT,  '|', '|'},

	// The alphabet
	{PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS,                  'a', 'A'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_UP,    'b', 'B'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_RIGHT, 'c', 'C'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_DOWN,  'd', 'D'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_LEFT,  'e', 'E'},

	{PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE,                  'f', 'F'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE | PSP_CTRL_UP,    'g', 'G'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE | PSP_CTRL_RIGHT, 'h', 'H'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE | PSP_CTRL_DOWN,  'i', 'I'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_CIRCLE | PSP_CTRL_LEFT,  'j', 'J'},

	{PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE,                  'k', 'K'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE | PSP_CTRL_UP,    'l', 'L'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE | PSP_CTRL_RIGHT, 'm', 'M'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE | PSP_CTRL_DOWN,  'n', 'N'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE | PSP_CTRL_LEFT,  'o', 'O'},

	{PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE,                  'p', 'P'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE | PSP_CTRL_UP,    'q', 'Q'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE | PSP_CTRL_RIGHT, 'r', 'R'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE | PSP_CTRL_DOWN,  's', 'S'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_TRIANGLE | PSP_CTRL_LEFT,  't', 'T'},

	{PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS,
	 'u', 'U'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_UP,
	 'v', 'V'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_RIGHT,
	 'w', 'W'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_DOWN,
	 'x', 'X'},
	{PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CROSS | PSP_CTRL_LEFT,
	 'y', 'Y'},

	{PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER | PSP_CTRL_CIRCLE, 'z', 'Z'},

	{0},
};

#endif // DOXYGEN_SKIP

//! Gets button pushes.
/**
 * This reports multiple button pushes, so this does not return until all
 * buttons have been released.
 *
 * @return An integer with button press flags.
 */
static int GetButtonPushes()
{
	int buttons;
	SceCtrlData pad;

	// Wait for a button press.
	while(1)
	{
		sceCtrlReadBufferPositive(&pad, 1);
		if(pad.Buttons) break;
	}

	// store the buttons.
	buttons = pad.Buttons;

	// wait for all buttons to come up, store them.
	while(pad.Buttons)
	{
		sceCtrlReadBufferPositive(&pad, 1);
		buttons |= pad.Buttons;
	}

	return buttons;
}

#ifndef DOXYGEN_SKIP

// Returns a key that matches the given buttons. Simple but fast on a small map.
static RPNPSPKey *FindKey(int buttons)
{
	int i;

	for(i = 0; KeyMap[i].buttons; i++)
		if(KeyMap[i].buttons == buttons)
			return &KeyMap[i];

	return NULL;
}

// 64 characters is a reasonable limit for the buffer.
#define RPN_BUF_SIZE 64
static char buffer[RPN_BUF_SIZE];
// To press "enter", press the triggers simultaneously.
static int enter = PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER;
// To cancel input, press the left trigger.
static int cancel = PSP_CTRL_LTRIGGER;
// To switch to alternate characters, press the right trigger.
static int alternate = PSP_CTRL_RTRIGGER;
// Don't start in alternate mode.
bool alternateMode = false;

// Clear the buffer to remove previous inputs.
static void clearInputBuffer()
{
	int i;

	for(i = 0; i < RPN_BUF_SIZE; i++)
		buffer[i] = 0;
}

#endif // DOXYGEN_SKIP

//! Get one character; map special characters so they can be handled later.
//!
/**
 * @return The typed character or 0 if invalid.
 */
static char GetCharacter()
{
	int buttons = GetButtonPushes();
	RPNPSPKey *key = FindKey(buttons);

	// Was a valid combination pressed? The print the character, then return it.
	if(key)
	{
		kprintf("%c", alternateMode ? key->alt_chr : key->chr);
		return alternateMode ? key->alt_chr : key->chr;
	}
	// Enter?
	else if(buttons == enter)
		return '\n';
	// Cancel?
	else if(buttons == cancel)
		return '\b';
	// Switch to alternate?
	else if(buttons == alternate)
		alternateMode = alternateMode ? false : true;
	// What happened?
	return 0;
}

//! Gets a string of input for the PSP.
/** @return The string.
 */
char *RPNPSP_GetString()
{
	int i, c = 0;

	// Clear the previous buffer before filling it.
	clearInputBuffer();

	// Go until the buffer is filled or a newline is returned.
	for(i = 0; i < RPN_BUF_SIZE && c != '\n'; i++)
	{
		c = GetCharacter();
		// Backspace?
		if(c == '\b')
		{
			kprintf("\n");
			return NULL;
		}
		// Real character?
		else if(c)
			buffer[i] = c;
		// No; stay back.
		else
			i--;
	}

	kprintf("\n");
	return buffer;
}
