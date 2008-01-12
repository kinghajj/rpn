#include "rpn.h"
#include <string.h>

bool RPN_argumentVersion(RPNCalculator *calculator, char **args)
{
	RPN_printf("%i.%i.%i.%i\n",
		__RPN_MAJOR__,
		__RPN_MINOR__,
		__RPN_REVIS__,
		__RPN_BUILD__);

	calculator->status = RPN_STATUS_EXIT;
	return false;
}

bool RPN_argumentHelp(RPNCalculator *calculator, char **args)
{
	RPN_printHelp();
	return false;
}

bool RPN_argumentExec(RPNCalculator *calculator, char **args)
{
	RPN_eval(args[0], calculator);
#ifdef RPN_LONG_DOUBLE
	RPN_printf("%Lf", RPN_peek(calculator->stack));
#elif  RPN_DOUBLE
	RPN_printf("%f", RPN_peek(calculator->stack));
#endif
	calculator->status = RPN_STATUS_EXIT;
	return true;
}

RPNArgument RPN_arguments[] =
{
	{"-v", "--version", 0, RPN_argumentVersion},
	{"-h", "--help", 0, RPN_argumentHelp},
	{"-e", "--exec", 1, RPN_argumentExec},
	{0},
};

RPNArgument *RPN_findArgument(char *name)
{
	int i;
	RPNArgument *argument;

	for(i = 0, argument = &RPN_arguments[i];
		argument->long_name;
		i++, argument = &RPN_arguments[i])
	{
		if(!strcmp(argument->short_name, name) ||
			!strcmp(argument->long_name, name))
			return argument;
	}

	return NULL;
}

void RPN_processArguments(RPNCalculator *calculator, int argc, char *argv[])
{
	int i;
	int cont = 1;
	RPNArgument *argument;

	for(i = 0; i < argc && cont; i++)
	{
		argument = RPN_findArgument(argv[i]);
		if(argument && argc - i >= argument->nargs)
			if(!argument->func(calculator, &argv[i+1])) cont = 0;
	}
}
