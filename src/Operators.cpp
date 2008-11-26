#include "rpn.h"
using namespace std;
using namespace RPN;

static Value addition(Value a, Value b)
{
    return a + b;
}

static Value subtraction(Value a, Value b)
{
    return a - b;
}

static Value multiplication(Value a, Value b)
{
    return a * b;
}

static Value division(Value a, Value b)
{
    return a / b;
}

Operators RPN::defaultOperators()
{
    Operators ret;

    ret["+"] = addition;
    ret["-"] = subtraction;
    ret["*"] = multiplication;
    ret["/"] = division;

    return ret;
}
