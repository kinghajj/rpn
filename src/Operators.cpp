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
 * Operators.cpp - operator functions.                                         *
 ******************************************************************************/

#include "rpn.h"
#include <cmath>
using namespace std;
using namespace RPN;

#ifndef DOXYGEN_SKIP

static Value addition(Value a, Value b)
{
    return a + b;
}

static Value subtraction(Value a, Value b)
{
    return a - b;
}

static Value multiplication(Value a, Value b)
{
    return a * b;
}

static Value division(Value a, Value b)
{
    return a / b;
}

static Value power(Value a, Value b)
{
    return powl(a, b);
}

static Value equals(Value a, Value b)
{
    return a == b ? 1 : 0;
}

static Value modulo(Value a, Value b)
{
    return (long)a % (long)b;
}

static Value _xor(Value a, Value b)
{
    return (long)a ^ (long)b;
}

static Value _and(Value a, Value b)
{
    return (long)a & (long)b;
}

static Value _or(Value a, Value b)
{
    return (long)a | (long)b;
}

#endif

Operators RPN::defaultOperators()
{
    Operators ret;

    ret["+"]  = addition;
    ret["-"]  = subtraction;
    ret["*"]  = multiplication;
    ret["/"]  = division;
    ret["**"] = power;
    ret["="]  = equals;
    ret["%"]  = modulo;
    ret["^"]  = _xor;
    ret["&"]  = _and;
    ret["|"]  = _or;

    return ret;
}
