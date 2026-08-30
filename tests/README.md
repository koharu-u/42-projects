# 42tester

`42tester` is developer tooling for exercising 42 C projects. It lives outside
submitted projects and never rewrites student source files.

Phase 1 supports Libft. The currently registered behavioural tests cover:

- `ft_strlen.basic`
- `ft_strlen.empty`
- `ft_strlen.boundary`
- `ft_strlen.random`
- basic checks for `ft_isalpha`, `ft_isdigit`, `ft_memset`, and `ft_memcpy`

The ncurses UI, JSON reports, symbol checks, Valgrind, malloc injection,
mutation fuzzing, and shrinking are intentionally deferred until later phases.

## Build and run

```sh
make -C tests
./tests/bin/42tester ./common-core/libft
./tests/bin/42tester --project libft ./common-core/libft
./tests/bin/42tester ./common-core/libft --no-ui
```

The default project mode requires a Makefile with a working `clean` target and
a default build that creates `libft.a`. The pipeline runs Norminette first,
then invokes:

```text
make -C <target> clean
make -C <target>
```

Norminette is resolved through `PATH`. If it is absent, its status is
`UNKNOWN`, not `FAIL`, and the clean build continues. A failed build stops
runtime testing.

## Direct sources and partial projects

Direct source mode never invokes `make` and does not need `libft.a` or the
rest of Libft. It links the chosen source, its metadata-declared dependencies,
and the smallest compatible test worker using `-Wall -Wextra -Werror`.

```sh
./tests/bin/42tester \
    --source ./common-core/libft/ft_strlen.c \
    --function ft_strlen
```

Use partial mode to scan known Libft filenames without trying the project
Makefile. Present functions are compiled independently; absent functions are
reported as `MISSING`, and a present source with an unavailable declared
dependency is reported as `BLOCKED` rather than failed.

```sh
./tests/bin/42tester --project libft ./common-core/libft --partial
./tests/bin/42tester --project libft ./common-core/libft \
    --function ft_memset --partial
```

The run header identifies `Build mode: PROJECT` or
`Build mode: DIRECT / PARTIAL`. Temporary workers are removed after each run;
pass `--keep-temp` to retain them and print their location.

## Deterministic random tests and replay

Every run displays its global seed. A random invocation derives its own test
seed from the global seed, stable test ID, and iteration number.

```sh
./tests/bin/42tester \
    --project libft \
    --test ft_strlen.random \
    --seed 183948291 \
    ./common-core/libft
```

To replay one reported iteration:

```sh
./tests/bin/42tester \
    --project libft \
    --test ft_strlen.random \
    --seed 183948291 \
    --iteration 51 \
    ./common-core/libft
```

Useful Phase 1 options:

```text
--project libft
--source <file.c>
--function <libft-function>
--partial
--keep-temp
--test <stable-test-id>
--seed <uint64>
--iterations <positive-count>
--iteration <uint64>
--timeout <milliseconds>
--no-ui
```

## Isolation model

The public `bin/42tester` executable contains no references to student
functions. After a successful clean build, it creates a temporary worker linked
to the target's `libft.a`. The worker registers tests and forks a new child for
every invocation. Only that child calls student code.

The parent worker concurrently captures stdout and stderr, enforces a monotonic
deadline, and maps `waitpid` state to structured `PASS`, `FAIL`, `CRASH`,
`TIMEOUT`, `SKIP`, or `ERROR` results. A versioned pipe protocol carries those
results to the headless frontend. The runner contains no terminal code.

Random iterations are separate child processes, so one crashing input does not
prevent later iterations from running.

## Exit status

- `0`: executed checks and tests passed; `UNKNOWN` optional tools are allowed
- `1`: Norminette failed, the build failed, or a student test failed/crashed/timed out
- `2`: CLI misuse or tester/module/protocol infrastructure error

Norminette failure affects the final exit status but does not stop the remaining
pipeline.
