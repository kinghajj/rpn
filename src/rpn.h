#ifndef _RPN_H_
#define _RPN_H_

#include <list>
#include <map>
#include <stack>
#include <string>
using namespace std;

namespace RPN
{
	// forward declarations.
	class Calculator;
	class Command;
	class Operator;

	typedef long double Value;
	typedef map<string, Command>   Commands;
	typedef map<string, Operator>  Operators;
	typedef map<string, Value>     Variables;
	typedef stack<Value>           Stack;
	typedef stack<Stack>           History;

	class Calculator
	{
		Commands  commands;
		History   history;
		Operators operators;
		Variables variables;
	public:
		Calculator();
		void Eval(string input);
	};

	class Operator
	{
	public:
		virtual operator()(Value a, Value b) = 0;
	};

	class Command
	{
	public:
		virtual unsigned int Arguments() = 0;
		virtual operator()(Calculator& calculator, list<string>& args) = 0;
	};
}

#endif // _RPN_H_
