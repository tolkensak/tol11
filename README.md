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

**Using tolc (Pure C)**

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

The library has evolved over nearly two decades of active use in production systems:

| Period | Focus |
|--------|-------|
| **2007–2010** | Foundation — `tolc` core library (C), early `tolcpp` classes |
| **2010–2013** | MFC extensions (`tolmfc`) for Windows GUI applications |
| **2013–2016** | `tolmysql` — database wrappers for enterprise systems |
| **2016–2020** | `tolxml` — XML processing for data interchange |
| **2020–present** | Modernization — C++11/14/17 compatibility, Visual Studio 2019+ support |

**Version 1.1** — current release, published publicly on GitHub in 2026.

The library was used across multiple projects in railway automation, warehouse management, GIS, and enterprise reporting systems.

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

## Contact

**Tolkyn Akhmetollauly**

- GitHub: [@tolkensak](https://github.com/tolkensak)

- Portfolio: [tolkensak.github.io/tolkensak](https://tolkensak.github.io/tolkensak)

- LinkedIn: [linkedin.com/in/tolkyn-akhmetollauly-0a3873a9](https://www.linkedin.com/in/tolkyn-akhmetollauly-0a3873a9)
