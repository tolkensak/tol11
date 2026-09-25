# tol11

> A multi-language utility library for C and C++ — in continuous development since 2007.

[![Language: C](https://img.shields.io/badge/language-C-blue.svg)]()
[![Language: C++](https://img.shields.io/badge/language-C%2B%2B-blue.svg)]()
[![Platform: Windows](https://img.shields.io/badge/platform-Windows-lightgrey.svg)]()
[![Version: 1.1](https://img.shields.io/badge/version-1.1-green.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

<br />

## Overview

**tol11** is a collection of reusable C and C++ libraries designed to implement the **DRY (Don't Repeat Yourself)** principle across multiple projects. It provides low-level primitives, utility classes, and framework extensions that simplify common development tasks.

The library has been actively developed and maintained since **2007** and supports multiple generations of Microsoft Visual Studio compilers.

<br />

## Libraries

| Library | Language | Purpose |
|---------|----------|---------|
| **tolc** | C | Core utilities, memory management, string handling, data structures |
| **tolcpp** | C++ | RAII wrappers, containers, algorithms built on tolc |
| **tolmfc** | C++ (MFC) | Extended MFC classes for Windows GUI development |
| **tolmysql** | C++ | MySQL wrappers and operations — simplified database access |
| **tolxml** | C++ | XML parsing and serialization |

<br />

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

<br />

## Requirements

- **Windows** (Windows API Win32 or x64)
- **Microsoft Visual Studio** (2003 or later)
- **MFC** (for `tolmfc`)
- **MySQL** (for `tolmysql`)
- Optional: **Windows CE** support for embedded targets

<br />

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/tolkensak/tol11.git
   ```

2. Open the project for your Visual Studio version (e.g., 180/ for VS 2019).

3.  Build the libraries you need (each library is a separate project).

4. Add the inc/ folder to your include path and link against the built libraries.

<br />

## Example Usage

### Using tolc (Pure C)

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

### Using tolcpp (C++)

```c++
#include <iostream>
#include <tolcpp_array.h>
#include <tolcpp_stringa.h>

int main()
{
    tol::Array<tol::StringA> arr;

    arr.Add("Hello");
    arr.Add("World!");

    arr.Insert(1, ", ");

    for (int i = 0; i < arr.Count(); i++) {
        std::cout << arr[i];
    }
}
```

### Using tolmfc (MFC)

A minimal MFC application using `tol::TWinApp` and `tol::MainWnd`:

```c++
// App.h header file

#include <tolmfc_winapp.h> // This should be placed in precompiled header file stdafx.h
#include <tolmfc_mainwnd.h> // This should be placed in precompiled header file stdafx.h


class App : public tol::TWinApp
{
public:
	App();
	virtual ~App();
	virtual BOOL InitInstance();

protected:
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
```

```c++
// App.cpp source file

#include "stdafx.h"
#include "App.h"
#include "MainWnd.h"

BEGIN_MESSAGE_MAP(App, tol::TWinApp)
END_MESSAGE_MAP()


App::App()
{
}

App::~App()
{
}

App theApp;	// The one and only App object


BOOL App::InitInstance()
{
	tol::TWinApp::InitInstance();

	SetRegistryKey();

	MainWnd* pFrame=new MainWnd;
	if(!pFrame)
		return FALSE;

	m_pMainWnd=pFrame;

	if(!pFrame->LoadFrame(IDR_MAIN, WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE, NULL, NULL))
		return FALSE;

	pFrame->ShowWindow(m_uStartedTimes==1?SW_SHOWNORMAL:m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;
}

int App::ExitInstance() 
{
	return tol::TWinApp::ExitInstance();
}
```

TWinApp provides its own AboutDlg, adjusts the application's registry key when calling the SetRegistryKey() method, and places recently opened files in a submenu.

### Using tolmysql

And using SmartPointer, which is in tolcpp

```c++
#include <tolcpp_array.h>
#include <tolcpp_pointer.h>
#include <tolcpp_stringa.h>
#include <tolmysql_mysql.h>

class User : public tol::SmartObject
{
public:
	User(int nId, char* pcName)
    {
        m_nId=nId;
        m_strName=pcName;
    }

    virtual ~User(){}

protected:
	int m_nId;
    tol::StringA m_strName;
};

typedef tol::SmartPointer<User> UserPtr;
typedef tol::Array<UserPtr> UserPtrArray;


UserPtrArray GetGroupUsers(int nGroupId)
{
	tol::MySQLConnectPtr pConn=new tol::MySQLConnect;

	tol::StringA strQuery;
    strQuery.Format("SELECT id, user FROM users WHERE group_id = %d", nGroupId);

	pConn->RealQuery(strQuery, strQuery.Len());

	tol::MySQLResultPtr pRes=pConn->StoreResult();

    UserPtrArray arrUsers;
	MYSQL_ROW row;

	while(row=pRes->FetchRow()) {
		arrUsers.Add(new User(atoi(row[0]), row[1]));
    }

    return arrUsers;
}
```

<br />

## Design Principles

- DRY (Don't Repeat Yourself) — shared functionality lives in one place

- Layered architecture — tolc → tolcpp → higher-level libraries

- Cross-version support — same API across 15+ years of compilers

- Minimal dependencies — no external libraries beyond the platform and MySQL

<br />

## History

| Year | Milestone |
|--- |---|
| 2007 | Project started — tolc C core library |
| 2009 | Visual Studio 2008 support; tolcpp classes added |
| 2015 | Visual Studio 2015 support |
| 2017 | Visual Studio 2017 support |
| 2019 | Visual Studio 2019 support |
| 2026 | Version 1.1 released publicly on GitHub |

<br />

## Status

| Library | Status |
|---------|--------|
| tolc | ✅ Stable |
| tolcpp | ✅ Stable |
| tolmfc | ✅ Stable |
| tolmysql | ✅ Stable |
| **tolxml** | ⚠️ **Requires MSXML update** — see issue tracker |

<br />

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

<br />

## Contact

**Tolkyn Akhmetollauly**

- GitHub: [@tolkensak](https://github.com/tolkensak)

- Portfolio: [tolkensak.github.io/tolkensak](https://tolkensak.github.io/tolkensak)

- LinkedIn: [linkedin.com/in/tolkyn-akhmetollauly-0a3873a9](https://www.linkedin.com/in/tolkyn-akhmetollauly-0a3873a9)
