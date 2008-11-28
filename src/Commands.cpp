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
 * Commands.cpp - command Calculator methods.                                  *
 ******************************************************************************/

#include "rpn.h"
#include <boost/typeof/typeof.hpp>
#include <cmath>
using namespace std;
using namespace RPN;

template <class T>
static void printAnything(T t)
{
    cout << t;
}

template <class T>
static void printAnyList(list<T>& l, void (*printer)(T))
{
    // list<T>::iterator doesn't work, so I'll use BOOST_AUTO.
    BOOST_AUTO(it, l.begin());

    cout << "[ ";
    while(it != l.end())
    {
        printer(*it++);
        cout << ", ";
    }
    cout << ']';
}

void Calculator::dup(list<string>& args)
{
    if(history.size() && history.front().size())
    {
        Stack& stack(history.front());
        Value dup = stack.front();
        stack.push_front(dup);
    }
}

void Calculator::exit(list<string>& args)
{
    status = Stop;
}

void Calculator::popHistory(list<string>& args)
{
    if(history.size() > 1)
        history.pop_front();
}

void Calculator::printHistory(list<string>& args)
{
    BOOST_AUTO(it, history.begin());

    cout << "[";
    while(it != history.end())
    {
        printAnyList(*it++, printAnything);
        cout << ", ";
    }
    cout << ']' << endl;
}

void Calculator::printStack(list<string>& args)
{
    if(HasStack())
    {
        printAnyList(history.front(), printAnything);
        cout << endl;
    }
}

void Calculator::pushHistory(list<string>& args)
{
    if(history.size())
    {
        Stack copy = history.front();
        history.push_front(copy);
    }
}

void Calculator::sqrtTop(list<string>& args)
{
    if(history.size() && history.front().size())
    {
        Value top = history.front().front();
        history.front().pop_front();
        history.front().push_front(sqrtl(top));
    }
}

void Calculator::swap(list<string>& args)
{
    if(history.size() && history.front().size() > 1)
    {
        Value a, b;
        a = history.front().front();
        history.front().pop_front();
        b = history.front().front();
        history.front().pop_front();
        history.front().push_front(a);
        history.front().push_front(b);
    }
}

void Calculator::unset(list<string>& args)
{
    variables.erase(args.front());
}

Commands Calculator::defaultCommands()
{
    Commands ret;

    ret["dup"]   = Command(&Calculator::dup);
    ret["poph"]  = Command(&Calculator::popHistory);
    ret["ph"]    = Command(&Calculator::printHistory);
    ret["ps"]    = Command(&Calculator::printStack);
    ret["pushh"] = Command(&Calculator::pushHistory);
    ret["sqrt"]  = Command(&Calculator::sqrtTop);
    ret["swap"]  = Command(&Calculator::swap);
    ret["unset"] = Command(&Calculator::unset, 1);
    ret["x"]     = Command(&Calculator::exit);

    return ret;
}
