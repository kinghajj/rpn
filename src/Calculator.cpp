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
 * Calculator.cpp - non-command Calculator methods.                            *
 ******************************************************************************/

#include "rpn.h"
#include <boost/tokenizer.hpp>
#include <sstream>
using namespace boost;
using namespace std;
using namespace RPN;

void Calculator::Eval(string s)
{
    typedef tokenizer< char_separator<char> > Tokens;
    char_separator<char> sep(" \t");
    Tokens tokens(s, sep);

    if(!HasStack()) return;

    for(Tokens::iterator tok = tokens.begin();
        tok != tokens.end() && status == Continue;
        ++tok)
    {
        Commands::iterator foundCommand    = commands.find(*tok);
        Operators::iterator foundOperator  = operators.find(*tok);
        Variables::iterator foundVariable  = variables.find(*tok);
        istringstream iss(*tok);
        Value val;

        // if the token is a number, push it.
        if(iss >> val)
            CurrentStack().push(val);

        // if the token is a command, perform it.
        else if(foundCommand != commands.end())
        {
            const Command& command = (*foundCommand).second;
            list<string> args;

            // collect a list of tokens that will be the arguments to the
            // command.
            while(tok != tokens.end() && args.size() != command.NumArgs())
                args.push_back(*++tok);

            // only perform a command if we can give it enough arguments.
            if(args.size() == command.NumArgs())
                command.Perform(*this, args);
        }

        // if the token is an operator and that stack has at least two items,
        // then perform that operator.
        else if(foundOperator != operators.end() && StackSize() > 1)
        {
            Value b = TopmostItem(); CurrentStack().pop();
            Value a = TopmostItem(); CurrentStack().pop();
            CurrentStack().push((*foundOperator).second(a, b));
        }

        // if the token is a variable, push the variable onto the stack.
        else if(foundVariable != variables.end())
            CurrentStack().push((*foundVariable).second);

        // otherwise, if the stack has at least one item, set a new variable 
        // whose name is the token and value is the top item.
        else variables[*tok] = TopmostItem();
    }
}

// displays the top item of the stack if there is one.
// I tried to write this as a friend operator<<(), but I got errors for
// accessing private data, which is what friend functions are supposed to be
// able to do!
ostream& Calculator::Display(ostream& os) const
{
    return os << TopmostItem();
}
