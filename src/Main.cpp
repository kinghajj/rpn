#include "rpn.h"
using namespace std;
using namespace RPN;

int main()
{
    Calculator calculator;

    while(calculator.IsRunning() && cin)
    {
        string s;
        cout << '[';
        calculator.Display(cout);
        cout << "]> ";
        getline(cin, s);
        calculator.Eval(s);
    }

    return 0;
}
