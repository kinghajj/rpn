#include "rpn.h"
using namespace std;
using namespace RPN;

static void exit(Calculator& calculator, list<string>& args)
{
    calculator.Exit();
}

Commands RPN::defaultCommands()
{
    Commands ret;

    ret["x"] = Command(exit);

    return ret;
}
