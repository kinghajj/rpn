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
 * rpn.h - common header for the entire program.                               *
 ******************************************************************************/

#ifndef _RPN_H_
#define _RPN_H_

#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <string>

//! Calls an objects member function via pointer.
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember)) 

namespace RPN
{
    // version information.
    const int         VERSION_MAJOR = 2;
    const int         VERSION_MINOR = 0;
    const int         VERSION_BUILD = 0;
    const std::string VERSION_EXTRA = "pre";

    // forward declarations.
    class Calculator;
    class Command;

    // various typedefs.
    typedef long double Value;
    typedef Value (*Operator)(Value a, Value b);
    typedef std::map<std::string, Command>   Commands;
    typedef std::map<std::string, Operator>  Operators;
    typedef std::map<std::string, Value>     Variables;
    typedef std::list<Value>                Stack;
    typedef std::list<Stack>                History;

    //! Returns a default, empty History stack.
    History   defaultHistory();
    //! Returns a map of the default operators.
    Operators defaultOperators();
    //! Returns a map of the default variables.
    Variables defaultVariables();

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
        History   history;
        Operators operators;
        Status    status;
        Variables variables;

        //! The command to duplicate the top item of the stack.
        void dup          (std::list<std::string>&);
        //! The command to exit the calculator.
        void exit         (std::list<std::string>&);
        //! Removes the top stack as long as there will be at least one left.
        void popHistory   (std::list<std::string>&);
        //! Copies the top stack and pushes it onto the history.
        void pushHistory  (std::list<std::string>&);
        //! The command to print the history.
        void printHistory (std::list<std::string>&);
        //! The command to print the stack.
        void printStack   (std::list<std::string>&);
        //! Pops the top item, then pushes its square root.
        void sqrtTop      (std::list<std::string>&);
        //! Swaps the top two items of the stack.
        void swap         (std::list<std::string>&);
        //! Unsets a previously set variable.
        void unset        (std::list<std::string>&);

        //! Returns true if there is at least one stack.
        bool HasStack() const { return history.size() != 0; }

        //! Returns a reference to the current stack.
        Stack& CurrentStack() { return history.front(); }

        //! Returns the current stack's size.
        size_t StackSize() const
        {
            return HasStack() ? history.front().size() : 0;
        }

        //! Returns the topmost item of the current stack.
        Value TopmostItem() const
        {
            return HasStack() && StackSize() > 0 ? history.front().front() : 0;
        }

    public:
        Calculator()
            : commands  (defaultCommands()),
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
        Commands  defaultCommands();

        //! Displays the top item of the stack.
        std::ostream& Display(std::ostream& os = std::cout) const;
    };

    typedef void (Calculator::*CommandPtr)(std::list<std::string>&);

    //! Holds information on a command, such as what its function is and how
    //! many arguments it takes.
    class Command
    {
        CommandPtr command_ptr;
        unsigned num_args;

    public:
        Command(CommandPtr command_ptr = NULL, unsigned int num_args = 0)
            : command_ptr(command_ptr), num_args(num_args)
        {
        }

        unsigned NumArgs() const { return num_args; }

        void Perform(Calculator& calculator, std::list<std::string> args) const
        {
            if(command_ptr)
                CALL_MEMBER_FN(calculator, command_ptr)(args);
        }
    };
}

#endif // _RPN_H_
