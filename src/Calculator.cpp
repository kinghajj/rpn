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
    
    for(Tokens::iterator tok = tokens.begin();
        tok != tokens.end() && status == Continue;
        ++tok)
    {
        Commands::iterator foundCommand    = commands.find(*tok);
        Operators::iterator foundOperator  = operators.find(*tok);
        Variables::iterator foundVariable  = variables.find(*tok);
        istringstream iss(*tok);
        Value val;

        // if the token is a number and there's a stack, push it.
        if(iss >> val && history.size() > 0)
            history.top().push(val);
        // if the token is a command, perform it.
        else if(foundCommand != commands.end())
                (*foundCommand).second.Perform(*this, list<string>());
        // if the token is an operator, there's a stack, and that stack has
        // at least two items, then perform that operator.
        else if(foundOperator != operators.end() && history.size() > 0 && 
                history.top().size() > 1)
        {
            Value b = history.top().top();
            history.top().pop();
            Value a = history.top().top();
            history.top().pop();
            history.top().push((*foundOperator).second(a, b));
        }
        // if the token is a variable and there's a stack, push the variable
        // onto the stack.
        else if(foundVariable != variables.end() && history.size() > 0)
            history.top().push((*foundVariable).second);
        // otherwise, if there's a stack with at least one item, set a new
        // variable whose name is the token and value is the top item.
        else if(history.size() > 0 && history.top().size() > 0)
            variables[*tok] = history.top().top();
    }
}

ostream& Calculator::Display(ostream& os) const
{
    if(history.size() > 0 && history.top().size() > 0)
        os << history.top().top();
    return os;
}
