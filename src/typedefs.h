/*******************************************************************************
 * Reverse Polish Notation calculator.                                         *
 * Copyright (c) 2007-2009, Samuel Fredrickson <kinghajj@gmail.com>            *
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
 * typedefs.h - header for typedefs or forward declarations.                   *
 ******************************************************************************/

#ifndef RPN_TYPEDEFS_H
#define RPN_TYPEDEFS_H

#include <list>
#include <map>
#include <string>
#include <vector>

namespace RPN
{
    ////////////////////////////////////////////////////////////////////////////
    // FORWARD DECLARATIONS                                                   //
    ////////////////////////////////////////////////////////////////////////////

    class Calculator;
    class Command;
    class HelpItem;

    ////////////////////////////////////////////////////////////////////////////
    // TYPEDEFS                                                               //
    ////////////////////////////////////////////////////////////////////////////

#ifdef RPN_DOUBLE
    //! The type operated on by the calculator.
    typedef double Value;
#elif  RPN_LONG_DOUBLE
    //! The type operated on by the calculator.
    typedef long double Value;
#else
#error Please choose either RPN_DOUBLE or RPN_LONG_DOUBLE.
#endif
    //! The type of an operator function.
    typedef Value (*Operator)(Value a, Value b);
    //! The type of a collection of commands.
    typedef std::map<std::string, Command>   Commands;
    //! The type of a collection of operators.
    typedef std::map<std::string, Operator>  Operators;
    //! The type of a collection of variables.
    typedef std::map<std::string, Value>     Variables;
    //! The type of the stack used by the calculator.
    typedef std::list<Value>                 Stack;
    //! The type of the history stack used by the calculator.
    typedef std::list<Stack>                 History;
    //! The type of a command member function. All commands must be members
    //! of the Calculator class.
    typedef void (Calculator::*CommandPtr)(std::vector<std::string>&);
    //! A list of help items.
    typedef std::list<HelpItem> HelpItems;
}

#endif
