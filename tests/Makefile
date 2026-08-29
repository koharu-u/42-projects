NAME := bin/42tester
CC ?= cc

CPPFLAGS := -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 \
	-DTESTER_SOURCE_ROOT=\"$(CURDIR)\" \
	-Iinclude -Isrc/internal -Imodules/libft
CFLAGS ?= -std=c11 -Wall -Wextra -Werror
LDFLAGS ?=
DEPFLAGS := -MMD -MP

HOST_SOURCES := \
	src/main.c \
	src/core/controller.c \
	src/core/event.c \
	src/core/module_registry.c \
	src/core/result.c \
	src/fuzz/rng.c \
	src/preflight/preflight.c \
	src/preflight/norminette.c \
	src/preflight/path_lookup.c \
	src/runtime/capture.c \
	src/runtime/event_wire.c \
	src/runtime/process.c \
	src/runtime/signal.c \
	src/runtime/timeout.c \
	src/runtime/worker_builder.c \
	src/report/terminal.c \
	modules/libft/module.c \
	modules/libft/worker_build.c

OBJ_DIR := .build/obj/host
HOST_OBJECTS := $(HOST_SOURCES:%.c=$(OBJ_DIR)/%.o)
HOST_DEPS := $(HOST_OBJECTS:.o=.d)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(HOST_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HOST_OBJECTS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(HOST_DEPS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -rf .build

re: fclean all
