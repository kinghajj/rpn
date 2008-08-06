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
 *  Forward declarations of structures to help with circular dependency woes.  *
 ******************************************************************************/

#ifndef RPN_STRUCTS_H
#define RPN_STRUCTS_H

struct RPNCalculator;
typedef struct RPNCalculator RPNCalculator;

struct RPNCommands;
typedef struct RPNCommands RPNCommands;

struct RPNCommand;
typedef struct RPNCommand RPNCommand;

struct RPNHistory;
typedef struct RPNHistory RPNHistory;

struct RPNNode;
typedef struct RPNNode RPNNode;

struct RPNOperator;
typedef struct RPNOperator RPNOperator;

struct RPNOperators;
typedef struct RPNOperators RPNOperators;

struct RPNStack;
typedef struct RPNStack RPNStack;

struct RPNTokens;
typedef struct RPNTokens RPNTokens;

struct RPNVariable;
typedef struct RPNVariable RPNVariable;

struct RPNVariables;
typedef struct RPNVariables RPNVariables;

#endif // RPN_STRUCTS_H
