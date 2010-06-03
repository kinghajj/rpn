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
 * Calculator.h - header for the Calculator class.                             *
 ******************************************************************************/

#ifndef RPN_CALCULATOR_H
#define RPN_CALCULATOR_H

#include <string>
#include <vector>
#include "typedefs.h"

namespace RPN
{
    //! The main class for the program.
    class Calculator
    {
        //! Holds the calculator's status, i.e. whether it's running or not.
        enum Status
        {
            Continue,
            Stop
        };

        Commands  commands;
        HelpItems helpItems;
        History   history;
        Operators operators;
        Status    status;
        Variables variables;

        //! The command to duplicate the top item of the stack.
        void dup                   (std::vector<std::string>&);
        //! The command to exit the calculator.
        void exit                  (std::vector<std::string>&);
        //! Pops the topmost item from the stack.
        void pop                   (std::vector<std::string>&);
        //! Removes the top stack as long as there will be at least one left.
        void popHistory            (std::vector<std::string>&);
        //! The command to print the help list.
        void printHelp             (std::vector<std::string>&);
        //! Copies the top stack and pushes it onto the history.
        void pushHistory           (std::vector<std::string>&);
        //! The command to print the history.
        void printHistory          (std::vector<std::string>&);
        //! The command to print the history in detail.
        void printHistoryDetailed(std::vector<std::string>&);
        //! The command to print the stack.
        void printStack            (std::vector<std::string>&);
        //! The command to print the stack in detail.
        void printStackDetailed    (std::vector<std::string>&);
        //! The command to print the variables.
        void printVariables        (std::vector<std::string>&);
        //! The command to print the variables in detail.
        void printVariablesDetailed(std::vector<std::string>&);
        void printVersion          (std::vector<std::string>&);
        //! Pops the top item, then pushes its square root.
        void sqrtTop               (std::vector<std::string>&);
        //! Swaps the top two items of the stack.
        void swap                  (std::vector<std::string>&);
        //! Unsets a previously set variable.
        void unset                 (std::vector<std::string>&);

        //! Returns true if there is at least one stack.
        bool HasStack() const { return history.size() != 0; }

        //! Returns a reference to the current stack.
        Stack& CurrentStack() { return history.front(); }
        const Stack& CurrentStack() const { return history.front(); }

        //! Returns the current stack's size.
        size_t StackSize() const
        {
            return HasStack() ? CurrentStack().size() : 0;
        }

        //! Returns the topmost item of the current stack.
        Value TopmostItem() const
        {
            return HasStack() && StackSize() > 0 ? CurrentStack().front() : 0;
        }

    public:

        //! The default and only constructor.
        Calculator()
            : commands  (defaultCommands()),
              helpItems (defaultHelpItems()),
              history   (defaultHistory()),
              operators (defaultOperators()),
              status    (Continue),
              variables (defaultVariables())
        {
        }

        //! Evaluates a string.
        void Eval(std::string input);

        //! Returns true if the calculator is running.
        bool IsRunning() const { return status == Continue; }

        //! Returns a map of the default commands.
        Commands defaultCommands();

        //! Displays the top item of the stack.
        void Display() const;
    };
}

#endif
