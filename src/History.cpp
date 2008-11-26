#include "rpn.h"
using namespace std;
using namespace RPN;

History RPN::defaultHistory()
{
    History ret;

    ret.push(Stack());

    return ret;
}
