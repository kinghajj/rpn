#include "rpn.h"
using namespace std;
using namespace RPN;

Variables RPN::defaultVariables()
{
    Variables ret;

    ret["E"]  = 2.718281828;
    ret["PI"] = 3.141592654;

    ret["KB"] = 1000;
    ret["MB"] = 1000 * 1000;
    ret["GB"] = 1000 * 1000 * 1000;
    ret["KiB"] = 1024;
    ret["MiB"] = 1024 * 1024;
    ret["GiB"] = 1024 * 1024 * 1024;

    ret["MINUTES"] = 60;
    ret["HOURS"]   = 60 * 60;

    return ret;
}
