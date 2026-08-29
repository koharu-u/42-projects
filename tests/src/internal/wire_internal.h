#ifndef WIRE_INTERNAL_H
# define WIRE_INTERNAL_H

# include "event.h"
# include "test_result.h"
# include <stdint.h>

# define TESTER_WIRE_MAGIC 0x34325453U
# define TESTER_WIRE_VERSION 2U

int	child_wire_send(int fd, t_test_status status, const char *diagnostic);
int	child_wire_receive(int fd, t_test_status *status, char **diagnostic,
		char **input_description);
int	event_wire_send_start(int fd, const t_test_invocation *invocation);
int	event_wire_send_result(int fd, const t_test_result *result);
int	event_wire_send_finished(int fd, const t_run_summary *summary);
int	event_wire_send_error(int fd, const char *message);
int	event_wire_receive_all(int fd, const t_event_sink *sink);

#endif
