# tol11

> A multi-language utility library for C and C++ — in continuous development since 2007.

[![Language: C](https://img.shields.io/badge/language-C-blue.svg)]()
[![Language: C++](https://img.shields.io/badge/language-C%2B%2B-blue.svg)]()
[![Platform: Windows](https://img.shields.io/badge/platform-Windows-lightgrey.svg)]()
[![Version: 1.1](https://img.shields.io/badge/version-1.1-green.svg)]()

---

## Overview

**tol11** is a collection of reusable C and C++ libraries designed to implement the **DRY (Don't Repeat Yourself)** principle across multiple projects. It provides low-level primitives, utility classes, and framework extensions that simplify common development tasks.

The library has been actively developed and maintained since **2007** and supports multiple generations of Microsoft Visual Studio compilers.

---

## Libraries

| Library | Language | Purpose |
|---------|----------|---------|
| **tolc** | C | Core utilities, memory management, string handling, data structures |
| **tolcpp** | C++ | RAII wrappers, containers, algorithms built on tolc |
| **tolmfc** | C++ (MFC) | Extended MFC classes for Windows GUI development |
| **tolmysql** | C++ | MySQL wrappers and operations — simplified database access |
| **tolxml** | C++ | XML parsing and serialization |

---

## Folder Structure

The repository is organized by **Visual Studio version**, reflecting the project's long history:

```
/
├── 71/    — Visual Studio 2003 (.NET 2003)
├── 90/    — Visual Studio 2008
├── 140/   — Visual Studio 2015
├── 170/   — Visual Studio 2017
├── 180/   — Visual Studio 2019
├── inc/   — Public headers
├── src/   — Source files
└── prj/   — Project files
```

Each version folder contains the project configuration for that compiler.

---

## Requirements

- **Windows** (Windows API (Win32 | X64), MFC)
- **Microsoft Visual Studio** (2003 or later)
- **MySQL** (for `tolmysql`)
- Optional: **Windows CE** support for embedded targets

---

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/tolkensak/tol11.git
   ```

2. Open the project for your Visual Studio version (e.g., 180/ for VS 2019).

3.  Build the libraries you need (each library is a separate project).

4. Add the inc/ folder to your include path and link against the built libraries.

## Example Usage

**tolc (Pure C)**

```c
#include <stdio.h>
#include <tolc_array.h>

int main()
{
	TARRAY arr = Array_Make(0);
	int i;

	Array_Add(arr, (TARRAYITEM)"Hello");
	Array_Add(arr, (TARRAYITEM)"World!");

	Array_Insert(arr, 1, (TARRAYITEM)", ");

	for (i = 0; i < Array_Count(arr); i++) {
		printf((char*)Array_At(arr, i));
	}

	Array_Kill(arr);
}
```

## Design Principles

- DRY (Don't Repeat Yourself) — shared functionality lives in one place

- Layered architecture — tolc → tolcpp → higher-level libraries

- Cross-version support — same API across 15+ years of compilers

- Minimal dependencies — no external libraries beyond the platform and MySQL

## History

| Year | Milestone |
|---|---|
| 2007 | Project started |
| 2009 | Visual Studio 2008 |
| 2015 | Visual Studio 2015 |
| 2017 | Visual Studio 2017 |
| 2018 | Visual Studio 2019 |
| 2026 | Version 1.1 released publicly on GitHub |

## License

(Add a license — e.g., MIT, Apache 2.0, or "All rights reserved." Without a license, others legally cannot use your code.)

## Contact

**Tolkyn Akhmetollauly**

- **GitHub:** [@tolkensak](@tolkensak)

- **Portfolio:** [tolkensak.github.io/tolkensak](tolkensak.github.io/tolkensak)

- **LinkedIn:** [linkedin.com/in/tolkyn-akhmetollauly-0a3873a9](linkedin.com/in/tolkyn-akhmetollauly-0a3873a9)
