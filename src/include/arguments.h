#ifndef RPN_ARGUMENTS_H
#define RPN_ARGUMENTS_H

#include <stdlib.h>

typedef bool (*RPNArgumentFunc)(RPNCalculator *calculator, char **args);

struct RPNArgument
{
	char *short_name;
	char *long_name;
	size_t nargs;
	RPNArgumentFunc func;
};

#endif // RPN_ARGUMENTS_H
