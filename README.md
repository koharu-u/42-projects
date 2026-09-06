# 42 Projects

## 📚 About

This repository contains my work from the **42 curriculum**, including both my **Piscine** experience and ongoing **Common Core** projects.

The repository is primarily used to:

* keep my 42 work organized in one place
* track my progress through the curriculum
* preserve previous exercises and projects
* make it easier to work across multiple machines
* document what I learn along the way

> [!NOTE]
> This repository contains my own implementations and learning process.
> If you are currently doing the same 42 projects, try solving them yourself before looking at someone else's solution.

---

## 🗂️ Repository Structure

```text
42-projects/
├── piscine/
│   ├── shell00/
│   ├── shell01/
│   ├── C00/
│   ├── C01/
│   ├── ...
│   ├── C09/
│   ├── rush00/
│   └── rush01/
│
├── common-core/
│   └── libft/
│
├── shell.nix
└── README.md
```

### 🏊 Piscine

[`piscine/`](./piscine)

Exercises and projects completed during the 42 Piscine.

Includes:

* Shell exercises
* C00 — C09
* Rush projects
* foundational C programming exercises

### 🧠 Common Core

[`common-core/`](./common-core)

Projects from the 42 Common Core curriculum.

| Project                        | Description                                                          | Status |
| ------------------------------ | -------------------------------------------------------------------- | :----: |
| [`libft`](./common-core/libft) | Reimplementation of common C library functions and utility functions |   ✅   |
| [`ft_printf`](https://github.com/koharu-u/42-projects/tree/feat/ft_printf/common-core/ft_printf) | Reimplementation of printf from libc |   ✏️   |

More projects will be added as I progress through the curriculum.

---

## 🛠️ Development Environment

Most projects are written in **C** and follow the constraints required by 42, including:

* Norminette coding style
* restricted standard-library usage
* Makefile-based builds
* manual memory management
* strict compiler warnings

A Nix development environment is also available through:

```bash
nix-shell
```

using the root [`shell.nix`](./shell.nix).

---

## 🌱 Progress

```text
Piscine      ████████████████████  archived
Common Core  ███░░░░░░░░░░░░░░░░  in progress
```

Current focus:

```text
common-core/libft
```

---

## 🧪 Philosophy

42 projects are less about producing the shortest possible solution and more about understanding what happens underneath.

This repository therefore reflects my learning process:

```text
read → understand → implement → break → debug → repeat
```

No magic. Just a suspicious amount of pointers.

---

## ⚠️ Disclaimer

This repository is intended primarily as a **personal archive and reference**.

If you are a 42 student working on these projects yourself, copying solutions will skip the part of the project that actually matters: figuring out why your code does not work at 3 AM.

Use it responsibly.
