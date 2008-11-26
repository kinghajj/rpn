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
#include <cmath>
using namespace std;
using namespace RPN;

template <class T>
static void printAnyStack(stack<T>& s)
{
    stack<T> copy = s;

    cout << "[";
    while(!copy.empty())
    {
        cout << copy.top() << ", ";
        copy.pop();
    }
    cout << "]" << endl;
}

void Calculator::dup(list<string>& args)
{
    if(history.size() && history.top().size())
    {
        Stack& stack(history.top());
        Value dup = stack.top();
        stack.push(dup);
    }
}

void Calculator::exit(list<string>& args)
{
    status = Stop;
}

void Calculator::popHistory(list<string>& args)
{
    if(history.size() > 1)
        history.pop();
}

void Calculator::printStack(list<string>& args)
{
    if(history.size())
    printAnyStack(history.top());
}

void Calculator::pushHistory(list<string>& args)
{
    if(history.size())
    {
        Stack copy = history.top();
        history.push(copy);
    }
}

void Calculator::sqrtTop(list<string>& args)
{
    if(history.size() && history.top().size())
    {
        Value top = history.top().top();
        history.top().pop();
        history.top().push(sqrtl(top));
    }
}

void Calculator::swap(list<string>& args)
{
    if(history.size() && history.top().size() > 1)
    {
        Value a, b;
        a = history.top().top();
        history.top().pop();
        b = history.top().top();
        history.top().pop();
        history.top().push(a);
        history.top().push(b);
    }
}

Commands Calculator::defaultCommands()
{
    Commands ret;

    ret["dup"]   = Command(&Calculator::dup);
    ret["poph"]  = Command(&Calculator::popHistory);
    ret["ps"]    = Command(&Calculator::printStack);
    ret["pushh"] = Command(&Calculator::pushHistory);
    ret["sqrt"]  = Command(&Calculator::sqrtTop);
    ret["swap"]  = Command(&Calculator::swap);
    ret["x"]     = Command(&Calculator::exit);

    return ret;
}
