# 42 Piscine — Rush 01 (Backup)

A small backup of the **Rush 01** project from the 42 Piscine.

## ✨ What’s inside

- **Language:** C (100%)
- **Goal:** Practice core C concepts (arrays, loops, basic algorithms, input/output)

## 🧰 Build & Run

> This repository is a backup and may not include a fully automated build system.

If there is a `Makefile`, you can usually do:

```sh
make
./<binary_name>
```

If there is no `Makefile`, a common approach is:

```sh
cc -Wall -Wextra -Werror *.c -o rush01
./rush01
```

## ⚠️ Caution: known memory leaks

This program is known to **leak memory**.

- Use **Valgrind** to inspect leaks:

```sh
valgrind --leak-check=full --show-leak-kinds=all ./rush01
```

- If you plan to reuse this code, consider:
  - Freeing all allocated memory paths
  - Avoiding unnecessary allocations
  - Adding cleanup on early returns / error paths

## 📌 Notes

- This repo is intended as a **backup/reference**, not a polished final deliverable.
- Feel free to open issues/PRs if you want to improve it.
