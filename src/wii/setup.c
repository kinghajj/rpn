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

/* setup.c - functions to setup Wii video and controllers, etc.
 *
 * This code is taken from the example template for devkitPPC with some editing.
 * I assume that since its a template, it's free to use without restriction.
 */

#include "rpn.h"
#include <fat.h>

#ifndef DOXYGEN_SKIP

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void RPNWii_Setup()
{
	// Initialise the video system.
	VIDEO_Init();
	// Initialise the attached controllers.
	WPAD_Init();
	// Allow the WPAD to use the accelerator ? (needed for libosk)
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	// Get the preferred video mode.
	rmode = VIDEO_GetPreferredMode(NULL);
	// Allocate memory for the display in the uncached region.
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	// Initialise the console (required for printf).
	console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight,
	             rmode->fbWidth * VI_DISPLAY_PIX_SZ);
	// Set up the video registers with the chosen mode.
	VIDEO_Configure(rmode);
	// Tell the video hardware where our display memory is.
	VIDEO_SetNextFramebuffer(xfb);
	// Make the display visible.
	VIDEO_SetBlack(FALSE);
	// Flush the video register changes to the hardware.
	VIDEO_Flush();
	// Wait for Video setup to complete.
	VIDEO_WaitVSync();
	if(rmode->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync();
	// Initialize the fat library.
	fatInitDefault();
}

#endif // DOXYGEN_SKIP
