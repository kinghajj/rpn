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
 * Argument.cpp - argument handling for the console port.                      *
 ******************************************************************************/

#include "../rpn.h"
using namespace RPN;
using namespace std;

static void argumentEvaluate(vector<string>& args)
{
    Calculator calculator;
    calculator.Eval(args[0]);
    calculator.Display();
    Print('\n');
}

static void argumentVersion(vector<string>& args)
{
    Print("RPN ");
    Print(VERSION_MAJOR);
    Print('.');
    Print(VERSION_MINOR);
    Print('.');
    Print(VERSION_REVIS);
    Print('.');
    Print(VERSION_BUILD);
    Print(' ');
    Print(VERSION_EXTRA);
    Print('\n');
}

//! Converts command-line arguments into a vector of strings.
vector<string> RPN::vectorize(char **argv, int argc)
{
    vector<string> ret;

    for(int i = 0; i < argc; ++i)
        ret.push_back(argv[i]);

    return ret;
}

bool RPN::processArguments(const vector<string>& args,
                           const Arguments& arguments)
{
    bool continueProgram = true;
    bool performed = false;

    // iterate through the arguments
    for(vector<string>::const_iterator it = args.begin();
        it != args.end() && continueProgram; it++)
    {
        Arguments::const_iterator found = arguments.find(*it);

        // if an argument was found in the map,
        if(found != arguments.end())
        {
            // if the argument requires arguments and there are enough,
            if(found->second.NumArgs() &&
               it + found->second.NumArgs() != args.end())
            {
                // create a sub-vector of the arguments,
                vector<string> argument_args(
                    it + 1, it + found->second.NumArgs() + 1);
                // and perform the argument.
                found->second.Perform(argument_args);
                performed = true;
                // then skip the argument's arguments.
                it += found->second.NumArgs();
            }
            // if the argument requires no arguments,
            else if(found->second.NumArgs() == 0)
            {
                // perform it with an empty arguments vector.
                vector<string> argument_args;
                found->second.Perform(argument_args);
                performed = true;
            }

            // ask the argument whether to continue the program.
            if(performed)
                continueProgram = found->second.ContinueProgram();
        }

        // the next argument is not yet performed.
        performed = false;
    }

    return continueProgram;
}

void RPN::setupArguments(Arguments& arguments)
{
    arguments["-e"]        = Argument(1, false, argumentEvaluate);
    arguments["-v"]        = Argument(0, false, argumentVersion);
    arguments["--version"] = Argument(0, false, argumentVersion);
}
