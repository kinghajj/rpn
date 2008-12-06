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
 * Help.cpp - the help module for the console port.                            *
 ******************************************************************************/

#include "rpn.h"
#include <boost/foreach.hpp>
using namespace RPN;
using namespace std;

void RPN::printHelpItems(const HelpItems& items)
{
    BOOST_FOREACH(const HelpItem& item, items)
    {
        Print("    ");
        Print(item.Brief());
        Print("\n        ");
        Print(item.Description());
        Print("\n");
    }
}

HelpItems RPN::defaultHelpItems()
{
    HelpItems items;

    items.push_back(HelpItem("+, -, *, /, **, sqrt, =",
                             "The basic math operators."));
    items.push_back(HelpItem("%, ^, &, |",
                             "Modulo and bitwise operators."));
    items.push_back(HelpItem("dup", "Pushes the topmost value to the stack."));
    items.push_back(HelpItem("pop", "Removes the topmost value of the stack."));
    items.push_back(HelpItem("ph",  "Prints the history stack."));
    items.push_back(HelpItem("phd", "Prints the history stack in detail."));
    items.push_back(HelpItem("ps",  "Prints the stack."));
    items.push_back(HelpItem("psd", "Prints the stack in detail."));
    items.push_back(HelpItem("ph",  "Prints the variable map."));
    items.push_back(HelpItem("phd", "Prints the variable map in detail."));
    items.push_back(HelpItem("x",   "Exits the program."));

    return items;
}
