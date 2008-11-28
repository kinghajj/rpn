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
#include <boost/foreach.hpp>
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
    cout << "[ ";
    BOOST_FOREACH(T& item, l)
        cout << item << ", ";
    cout << ']';
}

void Calculator::dup(vector<string>& args)
{
    if(history.size() && history.front().size())
    {
        Stack& stack(history.front());
        Value dup = stack.front();
        stack.push_front(dup);
    }
}

void Calculator::exit(vector<string>& args)
{
    status = Stop;
}

void Calculator::pop(vector<string>& args)
{
    CurrentStack().pop_front();
}

void Calculator::popHistory(vector<string>& args)
{
    if(history.size() > 1)
        history.pop_front();
}

void Calculator::printHistory(vector<string>& args)
{
    cout << '[';
    BOOST_FOREACH(Stack& item, history)
    {
        printAnyList(item, printAnything);
        cout << ", ";
    }
    cout << ']' << endl;
}

void Calculator::printStack(vector<string>& args)
{
    if(HasStack())
    {
        printAnyList(history.front(), printAnything);
        cout << endl;
    }
}

void Calculator::printVariables(vector<string>& args)
{
    cout << "[ ";
    BOOST_FOREACH(Variables::value_type& v, variables)
        cout << v.first << " = " << v.second << ", ";
    cout << ']' << endl;
}

void Calculator::pushHistory(vector<string>& args)
{
    if(history.size())
    {
        Stack copy = history.front();
        history.push_front(copy);
    }
}

void Calculator::sqrtTop(vector<string>& args)
{
    if(history.size() && history.front().size())
    {
        Value top = history.front().front();
        history.front().pop_front();
        history.front().push_front(sqrtl(top));
    }
}

void Calculator::swap(vector<string>& args)
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

void Calculator::unset(vector<string>& args)
{
    variables.erase(args.front());
}

Commands Calculator::defaultCommands()
{
    Commands ret;

    ret["dup"]   = Command(&Calculator::dup);
    ret["pop"]   = Command(&Calculator::pop);
    ret["poph"]  = Command(&Calculator::popHistory);
    ret["ph"]    = Command(&Calculator::printHistory);
    ret["ps"]    = Command(&Calculator::printStack);
    ret["pv"]    = Command(&Calculator::printVariables);
    ret["pushh"] = Command(&Calculator::pushHistory);
    ret["sqrt"]  = Command(&Calculator::sqrtTop);
    ret["swap"]  = Command(&Calculator::swap);
    ret["unset"] = Command(&Calculator::unset, 1);
    ret["x"]     = Command(&Calculator::exit);

    return ret;
}
