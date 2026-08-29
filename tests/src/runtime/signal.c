#include "test_result.h"
#include <signal.h>

const char	*test_signal_name(int signal_number)
{
	if (signal_number == SIGSEGV)
		return ("SIGSEGV");
	if (signal_number == SIGABRT)
		return ("SIGABRT");
	if (signal_number == SIGBUS)
		return ("SIGBUS");
	if (signal_number == SIGFPE)
		return ("SIGFPE");
	if (signal_number == SIGILL)
		return ("SIGILL");
	if (signal_number == SIGKILL)
		return ("SIGKILL");
	if (signal_number == SIGTERM)
		return ("SIGTERM");
	return ("UNKNOWN_SIGNAL");
}

