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
 * Commands.cpp - command Calculator methods.                                  *
 ******************************************************************************/

#include "rpn.h"
#include <boost/foreach.hpp>
#include <cmath>
using namespace std;
using namespace RPN;

#ifndef DOXYGEN_SKIP

template <class T>
static void printAnything(T t)
{
    Print(t);
}

template <class T>
static void printAnythingDetailed(T t)
{
    PrintDetailed(t);
}

template <class T>
static void printAnyList(list<T>& l, void (*printer)(T))
{
    Print("[ ");
    BOOST_FOREACH(T& item, l)
    {
        printer(item);
        Print(", ");
    }
    Print(']');
}

static void printValue(Value v)
{
    printAnything(v);
}

static void printValueDetailed(Value v)
{
    printAnythingDetailed(v);
}

void Calculator::dup(vector<string>&)
{
    if(HasStack() && StackSize() > 0)
    {
        Stack& stack(CurrentStack());
        stack.push_front(stack.front());
    }
}

void Calculator::exit(vector<string>&)
{
    status = Stop;
}

void Calculator::pop(vector<string>&)
{
    if(HasStack() && StackSize() > 0)
        CurrentStack().pop_front();
}

void Calculator::popHistory(vector<string>&)
{
    if(history.size() > 1)
        history.pop_front();
}

void Calculator::printHelp(vector<string>&)
{
    printHelpItems(helpItems);
}

void Calculator::printHistory(vector<string>&)
{
    Print('[');
    BOOST_FOREACH(Stack& item, history)
    {
        printAnyList(item, printValue);
        Print(", ");
    }
    Print("]\n");
}

void Calculator::printHistoryDetailed(vector<string>&)
{
    Print('[');
    BOOST_FOREACH(Stack& item, history)
    {
        printAnyList(item, printValueDetailed);
        Print(", ");
    }
    Print("]\n");
}

void Calculator::printStack(vector<string>&)
{
    if(HasStack())
    {
        printAnyList(CurrentStack(), printValue);
        Print('\n');
    }
}

void Calculator::printStackDetailed(vector<string>&)
{
    if(HasStack())
    {
        printAnyList(CurrentStack(), printValueDetailed);
        Print('\n');
    }
}

void Calculator::printVariables(vector<string>&)
{
    Print("[ ");
    BOOST_FOREACH(Variables::value_type& v, variables)
    {
        Print(v.first);
        Print(" = ");
        Print(v.second);
        Print(", ");
    }
    Print("]\n");
}

void Calculator::printVariablesDetailed(vector<string>&)
{
    Print("[ ");
    BOOST_FOREACH(Variables::value_type& v, variables)
    {
        Print(v.first);
        Print(" = ");
        PrintDetailed(v.second);
        Print(", ");
    }
    Print("]\n");
}

void Calculator::printVersion(vector<string>&)
{
    Port::Print("RPN %i.%i.%i.%i", VERSION_MAJOR, VERSION_MINOR,
                                   VERSION_REVIS, VERSION_BUILD);
    Port::Print(" (%s) ", getVersion());

#ifdef __GNUC__
    Port::Print("(GCC %i.%i.%i on %s at %s)", __GNUC__,
	                                          __GNUC_MINOR__,
	                                          __GNUC_PATCHLEVEL__,
	                                          __DATE__, __TIME__);
#endif
    Print("\nBy Sam Fredrickson <kinghajj@gmail.com>\n");
}

void Calculator::pushHistory(vector<string>&)
{
    if(HasStack())
        history.push_front(CurrentStack());
}

void Calculator::sqrtTop(vector<string>&)
{
    if(HasStack() && StackSize() > 0)
    {
        Value top = CurrentStack().front();
        CurrentStack().pop_front();
#ifdef RPN_DOUBLE
        CurrentStack().push_front(sqrt(top));
#elif  RPN_LONG_DOUBLE
        CurrentStack().push_front(sqrtl(top));
#endif
    }
}

void Calculator::swap(vector<string>&)
{
    if(HasStack() && StackSize() > 1)
    {
        Value a, b;
        a = CurrentStack().front();
        CurrentStack().pop_front();
        b = CurrentStack().front();
        CurrentStack().pop_front();
        CurrentStack().push_front(a);
        CurrentStack().push_front(b);
    }
}

void Calculator::unset(vector<string>& args)
{
    variables.erase(args.front());
}

#endif

Commands Calculator::defaultCommands()
{
    Commands ret;

    ret["dup"]   = Command(&Calculator::dup);
    ret["help"]  = Command(&Calculator::printHelp);
    ret["pop"]   = Command(&Calculator::pop);
    ret["poph"]  = Command(&Calculator::popHistory);
    ret["ph"]    = Command(&Calculator::printHistory);
    ret["phd"]   = Command(&Calculator::printHistoryDetailed);
    ret["ps"]    = Command(&Calculator::printStack);
    ret["psd"]   = Command(&Calculator::printStackDetailed);
    ret["pv"]    = Command(&Calculator::printVariables);
    ret["pvd"]   = Command(&Calculator::printVariablesDetailed);
    ret["pushh"] = Command(&Calculator::pushHistory);
    ret["sqrt"]  = Command(&Calculator::sqrtTop);
    ret["swap"]  = Command(&Calculator::swap);
    ret["unset"] = Command(&Calculator::unset, 1);
    ret["ver"]   = Command(&Calculator::printVersion);
    ret["x"]     = Command(&Calculator::exit);

    return ret;
}
