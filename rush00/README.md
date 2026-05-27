# 42 Piscine Rush 00

## Project Overview
This repository is a backup of a 42 Piscine **Rush 00** project written in C. The code generates ASCII rectangles using different border/corner rule sets (`rush00` through `rush04`) and prints them directly to standard output.

## Key Concepts and Architecture
- **Procedural decomposition:** each variant is split into `main.c`, `rushXX.c`, and `ft_putchar.c`.
- **Grid traversal algorithm:** nested `while` loops iterate over `(x, y)` coordinates to build each rectangle.
- **Rule-based character selection:** condition branches choose corner, edge, or interior characters based on current position.
- **Low-level output model:** all rendering is funneled through `ft_putchar`, a thin wrapper over the `write` system call.

## Notable Modules and Components
- `ex00/rush04.c`: primary exercise implementation of the rectangle renderer.
- `bonus/rush00` to `bonus/rush03`: alternate renderer variants with different border/corner character rules.
- `*/ft_putchar.c`: shared output primitive used by each rush variant.
- `*/main.c`: minimal entry points that call `rush(4, 4)`.

## Coding Standards and Constraints Reflected in the Codebase
- **C-only implementation** with no external libraries beyond standard system headers (`unistd.h`).
- **Function-focused style** with explicit forward declarations and small source files.
- **42-style formatting conventions** visible in file headers and naming (`ft_` prefix, `rush` naming).
- **No dynamic memory or complex data structures**; logic is implemented through control flow and coordinate checks.

## Repository Structure
- `ex00/` — core rush exercise (`rush04`).
- `bonus/rush00/` — variant 00 renderer.
- `bonus/rush01/` — variant 01 renderer.
- `bonus/rush02/` — variant 02 renderer.
- `bonus/rush03/` — variant 03 renderer.

## Skills Demonstrated
- Translating geometric rules into deterministic coordinate-based conditions.
- Building reusable output abstractions in low-level C.
- Organizing small C programs into clear, modular source units.
- Practicing disciplined control-flow reasoning for edge and corner cases.
