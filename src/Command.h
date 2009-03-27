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
 * Command.h - header for the Command class.                                   *
 ******************************************************************************/

#ifndef RPN_COMMAND_H
#define RPN_COMMAND_H

#include "typedefs.h"

namespace RPN
{
    //! Holds information on a command, such as what its function is and how
    //! many arguments it takes.
    class Command
    {
        CommandPtr command_ptr;
        unsigned num_args;

    public:

        //! Constructs a command with a function pointer and number of
        //arguments.
        Command(CommandPtr command_ptr = NULL, unsigned int num_args = 0)
            : command_ptr(command_ptr), num_args(num_args)
        {
        }

        //! Returns the number of arguments the function requires.
        unsigned NumArgs() const { return num_args; }

        //! Performs the command on a calculator with the given functions.
        void Perform(Calculator& calc, std::vector<std::string> args) const
        {
            if(command_ptr)
                CALL_MEMBER_FN(calc, command_ptr)(args);
        }
    };
}

#endif
