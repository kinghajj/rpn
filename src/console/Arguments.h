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
 * Arguments.h - argument handling for the console port.                       *
 ******************************************************************************/

#ifndef RPN_CONSOLE_ARGUMENT
#define RPN_CONSOLE_ARGUMENT

#include <map>
#include <string>
#include <vector>
#include "../typedefs.h"

namespace RPN
{
    class Argument
    {
        unsigned nargs;
        bool     continueProgram;
        void     (*f)(std::vector<std::string>&, Calculator&);

    public:
        Argument()
            : nargs(0), continueProgram(true), f(NULL)
        {
        }

        Argument(unsigned nargs, bool continueProgram,
                 void (*f)(std::vector<std::string>&, Calculator&))
            : nargs(nargs), continueProgram(continueProgram), f(f)
        {
        }

        bool ContinueProgram() const
        {
            return continueProgram;
        }

        unsigned NumArgs() const
        {
            return nargs;
        }

        void Perform(std::vector<std::string>& args, Calculator& calc) const
        {
            if(f) f(args, calc);
        }
    };

    typedef std::map<std::string, Argument> Arguments;

    std::vector<std::string> vectorize(char **argv, int argc);
    bool processArguments(const std::vector<std::string>& args,
                          const Arguments& arguments,
                          Calculator& calculator);
    void setupArguments(Arguments& arguments);
}

#endif
