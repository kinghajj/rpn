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
 * This is stack-based, so expressions can be as long as memory will allow.    *
 *   Operators must be one-character long. You can add your own by editing     *
 * the RPN_operator function in parser.c.                                      *
 *   I've commented this code to explain any 'magic' or otherwise cryptic      *
 * sections. I hope they will suffice.                                         *
 ******************************************************************************/

#ifndef __RPN_H__
#define __RPN_H__

#include <stdbool.h>

/* This wonderful header provides hashing for C structures. I use it to
 * implement a hash table to store and find variables, commands, and operators.
 *
 * No library is needed; it is implemented entirely as C macros in the header.
 */
#include "uthash.h"

// versioning
//! The major version number.
#define __RPN_MAJOR__ 1
//! The minor version number.
#define __RPN_MINOR__ 2
//! The revision version number.
#define __RPN_REVIS__ 3
//! The build version number.
#define __RPN_BUILD__ 1

//! Handy-dandy macro for allocating structures
#define new(x) (x*)malloc(sizeof(x))

// setup the type of RPNValue
#ifdef RPN_LONG_DOUBLE
typedef long double RPNValue;
#define RPN_VALUE_LONG_FORMAT  "%Lf"
#define RPN_VALUE_SHORT_FORMAT "%Lg"
#define RPN_strtod strtold
#elif  RPN_DOUBLE
typedef double RPNValue;
#define RPN_VALUE_LONG_FORMAT  "%f"
#define RPN_VALUE_SHORT_FORMAT "%g"
#define RPN_strtod strtod
#else
#error Please define either RPN_LONG_DOUBLE or RPN_DOUBLE.
#endif

// header for circular-dependency of structures.
#include "structs.h"

// headers for structures and their methods.
#include "arguments.h"
#include "calculator.h"
#include "commands.h"
#include "error.h"
#include "help.h"
#include "operators.h"
#include "parser.h"
#include "stack.h"
#include "tokens.h"
#include "variables.h"

/* Conditionally include PSP declarations.
 */
#ifdef RPN_PSP
#include "psp/psp.h"
#endif // RPN_PSP

#endif // __RPN_H__
