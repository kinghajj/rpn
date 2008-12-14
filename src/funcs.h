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
 * funcs.h - various useful functions that don't fit anywhere else.            *
 ******************************************************************************/

#ifndef RPN_FUNCS_H
#define RPN_FUNCS_H

#include "typedefs.h"
#include "HelpItem.h"
#include <sstream>

//! Calls an objects member function via pointer.
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember)) 

namespace RPN
{
    //! Returns a default, empty History stack.
    History   defaultHistory();
    //! Returns a map of the default operators.
    Operators defaultOperators();
    //! Returns a map of the default variables.
    Variables defaultVariables();
    //! Returns a list of the default help items.
    HelpItems defaultHelpItems();
    //! Portably prints a list of help items.
    void printHelpItems(const HelpItems& items);

    //! A portable way to print things.
    template <class T>
    void Print(const T& item)
    {
        std::ostringstream oss;
        oss << item;
        Port::Print(oss.str().c_str());
    }

    //! A portable way to print things in detail.
    template <class T>
    void PrintDetailed(const T& item)
    {
        std::ostringstream oss;
        oss << std::fixed << item;
        Port::Print(oss.str().c_str());
    }
}

#endif
