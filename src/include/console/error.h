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
 * Error-handling. (error.c)                                                   *
 *                                                                             *
 * If a GUI version of this app is ever made, then it may need to redifine     *
 * these functions. In that case, then I'll probably need to further separate  *
 * this into include/error.h, for standard error prototypes, and               *
 * include/console/error.h, for console error prototypes.                      *
 ******************************************************************************/

#ifndef RPN_ERROR_H
#define RPN_ERROR_H

void RPN_error(char *msg);
int RPN_printf(char *fmt, ...);
int RPN_dprintff(char *func, char *fmt, ...);

#ifdef RPN_DEBUG
//! Macro to print debug messages. If RPN_DEBUG not defined, then this call
//! doesn't do anything. So, use this as liberally as you want, because there
//! won't be any performance impact unless you compile for debugging.
#define RPN_dprintf(fmt, ...) \
	RPN_dprintff ((char*)__FUNCTION__, fmt, ## __VA_ARGS__)
void *RPN_malloc(size_t n);
void RPN_free(void *ptr);

#else // NOT debug mode, so just redefine these as macros to get optimized.

#define RPN_dprintf(fmt, ...) ; // don't even compile this
#define RPN_malloc malloc       // replace these with malloc/free
#define RPN_free free
#endif

#endif // RPN_ERROR_H