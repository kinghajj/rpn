#ifndef _RPN_H_
#define _RPN_H_

#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <string>

namespace RPN
{
    // forward declarations.
    class Calculator;
    class Command;

    // various typedefs.
    typedef long double Value;
    typedef Value (*Operator)(Value a, Value b);
    typedef void (*CommandPtr)(Calculator& calculator,
                               std::list<std::string>& args);
    typedef std::map<std::string, Command>   Commands;
    typedef std::map<std::string, Operator>  Operators;
    typedef std::map<std::string, Value>     Variables;
    typedef std::stack<Value>                Stack;
    typedef std::stack<Stack>                History;

    // these functions return defaults for various types.
    Commands  defaultCommands();
    History   defaultHistory();
    Operators defaultOperators();
    Variables defaultVariables();

    enum Status
    {
        Continue,
        Stop
    };

    class Calculator
    {
        Commands  commands;
        History   history;
        Operators operators;
        Status    status;
        Variables variables;

    public:
        Calculator()
            : commands  (defaultCommands()),
              history   (defaultHistory()),
              operators (defaultOperators()),
              status    (Continue),
              variables (defaultVariables())
        {
        }

        void Eval(std::string input);
        void Exit() { status = Stop; }
        bool IsRunning() const { return status == Continue; }

        friend std::ostream& operator<<(std::ostream&, const Calculator&);
        std::ostream& Display(std::ostream& os) const;
    };

    class Command
    {
        CommandPtr command_ptr;
        unsigned int num_args;

    public:
        Command(CommandPtr command_ptr = NULL, unsigned int num_args = 0)
            : command_ptr(command_ptr), num_args(num_args)
        {
        }

        void Perform(Calculator& calculator, std::list<std::string> args)
        {
            if(command_ptr) command_ptr(calculator, args);
        }
    };
}

#endif // _RPN_H_
