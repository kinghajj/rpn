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
#include <sstream>
#include <stack>
#include <string>
#include <vector>

//! Calls an objects member function via pointer.
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember)) 

namespace RPN
{
    ////////////////////////////////////////////////////////////////////////////
    // CONSTANTS                                                              //
    ////////////////////////////////////////////////////////////////////////////

    //! The major version number.
    const int         VERSION_MAJOR = 2;
    //! The minor version number.
    const int         VERSION_MINOR = 0;
    //! The revision version number.
    const int         VERSION_REVIS = 0;
    //! The build version number.
    const int         VERSION_BUILD = 0;
    //! Any extra information about the version.
    const std::string VERSION_EXTRA = "alpha";

    ////////////////////////////////////////////////////////////////////////////
    // FORWARD DECLARATIONS                                                   //
    ////////////////////////////////////////////////////////////////////////////

    class Calculator;
    class Command;

    ////////////////////////////////////////////////////////////////////////////
    // TYPEDEFS                                                               //
    ////////////////////////////////////////////////////////////////////////////

#ifdef RPN_DOUBLE
    //! The type operated on by the calculator.
    typedef double Value;
#elif  RPN_LONG_DOUBLE
    //! The type operated on by the calculator.
    typedef long double Value;
#else
#error Please choose either RPN_DOUBLE or RPN_LONG_DOUBLE.
#endif
    //! The type of an operator function.
    typedef Value (*Operator)(Value a, Value b);
    //! The type of a collection of commands.
    typedef std::map<std::string, Command>   Commands;
    //! The type of a collection of operators.
    typedef std::map<std::string, Operator>  Operators;
    //! The type of a collection of variables.
    typedef std::map<std::string, Value>     Variables;
    //! The type of the stack used by the calculator.
    typedef std::list<Value>                 Stack;
    //! The type of the history stack used by the calculator.
    typedef std::list<Stack>                 History;
    //! The type of a command member function. All commands must be members
    //! of the Calculator class.
    typedef void (Calculator::*CommandPtr)(std::vector<std::string>&);

    ////////////////////////////////////////////////////////////////////////////
    // FUNCTIONS                                                              //
    ////////////////////////////////////////////////////////////////////////////

    //! Returns a default, empty History stack.
    History   defaultHistory();
    //! Returns a map of the default operators.
    Operators defaultOperators();
    //! Returns a map of the default variables.
    Variables defaultVariables();

    ////////////////////////////////////////////////////////////////////////////
    // PORTABILITY                                                            //
    ////////////////////////////////////////////////////////////////////////////

#ifdef RPN_CONSOLE
#include "console/port.h"
#elif  RPN_PSP
#include "psp/port.h"
#elif  RPN_WII
#include "wii/port.h"
#else
#error Please choose a port to build.
#endif

    //! A portable way to print things.
    template <class T>
    void Print(const T& item)
    {
        std::ostringstream oss;
        oss << item;
        Port::Print(oss.str().c_str());
    }

    //! A portable way to print things in detail.
    template <class T>
    void PrintDetailed(const T& item)
    {
        std::ostringstream oss;
        oss << std::fixed << item;
        Port::Print(oss.str().c_str());
    }

    ////////////////////////////////////////////////////////////////////////////
    // CLASSSES                                                               //
    ////////////////////////////////////////////////////////////////////////////

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
        void dup                   (std::vector<std::string>&);
        //! The command to exit the calculator.
        void exit                  (std::vector<std::string>&);
        //! Pops the topmost item from the stack.
        void pop                   (std::vector<std::string>&);
        //! Removes the top stack as long as there will be at least one left.
        void popHistory            (std::vector<std::string>&);
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
        //! Pops the top item, then pushes its square root.
        void sqrtTop               (std::vector<std::string>&);
        //! Swaps the top two items of the stack.
        void swap                  (std::vector<std::string>&);
        //! Unsets a previously set variable.
        void unset         (std::vector<std::string>&);

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

        //! The default and only constructor.
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
        void Display() const;
    };

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

#endif // _RPN_H_
