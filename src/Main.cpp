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
 * Main.cpp - the entry point to the program.                                  *
 ******************************************************************************/

#include "rpn.h"
#include <vector>
using namespace std;
using namespace RPN;

static vector<string> vectorize(char **argv, int argc)
{
    vector<string> ret;

    for(int i = 0; i < argc; ++i)
        ret.push_back(argv[i]);

    return ret;
}

int main(int argc, char *argv[])
{
    Calculator calculator;
    vector<string> args = vectorize(argv, argc);

    if(args.size() > 1 && args[1] == "--version")
    {
        Print("RPN ");
        Print(VERSION_MAJOR);
        Print('.');
        Print(VERSION_MINOR);
        Print('.');
        Print(VERSION_BUILD);
        Print(' ');
        Print(VERSION_EXTRA);
        Print('\n');
        return 0;
    }

    while(calculator.IsRunning() && cin)
    {
        string s;
        Print('[');
        calculator.Display();
        Print("]> ");
        getline(cin, s);
        calculator.Eval(s);
    }

    return 0;
}
