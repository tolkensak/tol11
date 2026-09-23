
#ifndef _INC_TOLC_DISP_H
#define _INC_TOLC_DISP_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <tolc.h>


TOLEXTC_BEGIN


TOLCDECL BOOL TOLAPI Hook_SubclassBegin(HINSTANCE hInstance, DWORD dwThreadID);
TOLCDECL BOOL TOLAPI Hook_SubclassEnd();


/*-------------------------------------------------------------------------
Message and command dispatch infrastructure. The following type
definitions and functions are used by the message and command dispatching
mechanism and do not need to be changed.
-------------------------------------------------------------------------*/

/* For NON-MDI applications, uncomment line 1 below and comment
line 2.  For MDI applications, uncomment line 2 below, comment
line 1, and then define hwndMDIClient as a global variable. */
#define hwndMDIClient NULL        /* (1) Stub for NON-MDI applications. */
/*extern HWND hwndMDIClient;*/     /* (2) For MDI applications.          */

/* Function pointer prototype for message handling functions. */
typedef LRESULT (*PFNMSG)(HWND, UINT, WPARAM, LPARAM);

/* Function pointer prototype for command handling functions. */
typedef LRESULT (*PFNCMD)(HWND, WORD, WORD, HWND);

/* Function pointer prototype for notify handling functions. */
typedef LRESULT (*PFNNTF)(HWND, WPARAM, LPARAM);

/* Enumerated type used to determine which default window procedure
should be called by the message- and command-dispatching mechanism
if a message or command is not handled explicitly. */
typedef enum enumDefWndProcType // Enumeration for Default Window Procedures.
{
	dwpNone,     // Do not call any default procedure.
	dwpWindow,   // Call DefWindowProc.
	dwpSubclass, // Call DefSubclassProc.
	dwpDialog,   // Call DefDlgProc (This should be used only for custom dialog boxes-standard dialog box use edwpNone).
	dwpMDIChild, // Call DefMDIChildProc.
	dwpMDIFrame, // Call DefFrameProc.
	dwpCustom
} T_DWPT;

typedef struct tagDefWndProc
{
	T_DWPT dwpt;
	WNDPROC* wp;
} T_DWP;

/* This structure maps messages to message handling functions. */
typedef struct tagMsgMap // Message Dispatch structure.
{
	UINT uMsg;
	PFNMSG pFn;
} T_MSGMAP, *PT_MSGMAP;

/* This structure contains all of the information that a window
procedure passes to DispMessage in order to define the message
dispatching behavior for the window. */
typedef struct tagMsgMapInf // Message Dipatch Information.
{
	int num;     // Number of message dispatch structs.
	PT_MSGMAP pmm; // Table of message dispatch structures.
	T_DWP dwp;     // Type of default window handler needed.
} T_MSGMAPINF, *PT_MSGMAPINF;

/* This structure maps command IDs to command handling functions. */
typedef struct tagCmdMap // Command Dispatch structure.
{
	WORD wCmd;
	PFNCMD pFn;
} T_CMDMAP, *PT_CMDMAP;

/* This structure contains all of the information that a command
message procedure passes to DispCommand in order to define the
command dispatching behavior for the window. */
typedef struct tagCmdMapInf // Command Dispatch Information.
{
	int num;     // Number of command dispatch structs.
	PT_CMDMAP pcm; // Table of command dispatch structures.
	T_DWP dwp;     // Type of default window handler needed.
} T_CMDMAPINF, *PT_CMDMAPINF;

/* This structure maps notify IDs to command handling functions. */
typedef struct tagNtfMap // Notify Dispatch structure.
{
	UINT uNtf;
	PFNNTF pFn;
} T_NTFMAP, *PT_NTFMAP;

/* This structure contains all of the information that a notify
message procedure passes to DispNotify in order to define the
notify dispatching behavior for the window. */
typedef struct tagNtfMapInf // notify Dispatch Information.
{
	int num;     // Number of notify dispatch structs.
	PT_NTFMAP pnm; // Table of notify dispatch structures.
	T_DWP dwp;     // Type of default window handler needed.
} T_NTFMAPINF, *PT_NTFMAPINF;

/* Message and command dispatching functions.  They look up messages
and commands in the dispatch tables and call the appropriate handler function. */
TOLCDECL LRESULT TOLAPI Disp_Message(PT_MSGMAPINF, HWND, UINT, WPARAM, LPARAM);
TOLCDECL LRESULT TOLAPI Disp_Command(PT_CMDMAPINF, HWND, WPARAM, LPARAM);
TOLCDECL LRESULT TOLAPI Disp_Notify(PT_NTFMAPINF, HWND, WPARAM, LPARAM);
TOLCDECL LRESULT TOLAPI Disp_Default(T_DWP, HWND, UINT, WPARAM, LPARAM);

/* Command State */

typedef int (*PFNRANGE)();
typedef void (*PFNSTT)(HWND, UINT, LPMENUITEMINFO);

typedef struct tagSttMap
{
	UINT uID;
	PFNRANGE pFnRange;
	PFNSTT pFn;
} T_STTMAP, *PT_STTMAP;

typedef struct tagSttMapInf
{
	int num;
	PT_STTMAP psm;
} T_STTMAPINF, *PT_STTMAPINF;

TOLCDECL void TOLAPI Disp_State(PT_STTMAPINF psmi, HWND hWnd, HMENU hMenu, HWND hWndToolbar, WPARAM wParam, LPARAM lParam);


TOLEXTC_END

#endif /* _INC_TOLC_DISP_H */
