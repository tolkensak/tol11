
#include "stdafx.h"
#include <tolc_dispatch.h>

#include <windowsx.h>
#include <commctrl.h>


TOLEXTC_BEGIN


#ifdef TOLCDLL
#pragma data_seg("Shared")
#endif

HHOOK lib_hHookSubclass=NULL;
//HHOOK lib_hHookForegroundIdle=NULL;

#ifdef TOLCDLL
#pragma data_seg()
#pragma comment(linker, "/section:Shared,RWS")
#endif

TOLCDECL LRESULT CALLBACK Hook_SubclassProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode==HCBT_CREATEWND)
	{
		LPCREATESTRUCT lpcs=((LPCBT_CREATEWND)lParam)->lpcs;
		if(lpcs->lpCreateParams && lpcs->hMenu && !SetWindowSubclass((HWND)wParam, lpcs->lpCreateParams, (UINT_PTR)lpcs->hMenu, 0))
			return 1;
	}

	return CallNextHookEx(lib_hHookSubclass, nCode, wParam, lParam);
}

//TOLCDECL LRESULT CALLBACK Hook_ForegroundIdleProc(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	if(nCode>=0)
//	{
//	}
//
//	return CallNextHookEx(lib_hHookForegroundIdle, nCode, wParam, lParam);
//}

TOLCDECL BOOL TOLAPI Hook_SubclassBegin(HINSTANCE hInstance, DWORD dwThreadID)
{
	if(!lib_hHookSubclass)
		lib_hHookSubclass=SetWindowsHookEx(WH_CBT, Hook_SubclassProc, hInstance, dwThreadID);

	return lib_hHookSubclass!=NULL;
}

TOLCDECL BOOL TOLAPI Hook_SubclassEnd()
{
	if(UnhookWindowsHookEx(lib_hHookSubclass))
		lib_hHookSubclass=NULL;

	return lib_hHookSubclass==NULL;
}

TOLCDECL LRESULT TOLAPI Disp_Default(T_DWP dwp, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(dwp.dwpt)
	{
	case dwpNone: return 0;
	case dwpWindow: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	case dwpSubclass: return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	case dwpDialog: return DefDlgProc(hWnd, uMsg, wParam, lParam);
	case dwpMDIFrame: return DefFrameProc(hWnd, hwndMDIClient, uMsg, wParam, lParam);
	case dwpMDIChild: return DefMDIChildProc(hWnd, uMsg, wParam, lParam);
	case dwpCustom: return CallWindowProc(*dwp.wp, hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

TOLCDECL LRESULT TOLAPI Disp_Message(PT_MSGMAPINF pmmi, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int i;

	for(i=0; i<pmmi->num; i++)
		if(pmmi->pmm[i].uMsg==uMsg)
			return pmmi->pmm[i].pFn(hWnd, uMsg, wParam, lParam);

	return Disp_Default(pmmi->dwp, hWnd, uMsg, wParam, lParam);
}

TOLCDECL LRESULT TOLAPI Disp_Command(PT_CMDMAPINF pcmi, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int i;
	WORD wCmd=GET_WM_COMMAND_ID(wParam, lParam);

	/* Message packing of wparam and lparam have changed for Win32,
	so use the GET_WM_COMMAND macro to unpack the commnad. */

	for(i=0; i<pcmi->num; i++)
		if(pcmi->pcm[i].wCmd==wCmd)
			return pcmi->pcm[i].pFn(hWnd, wCmd, GET_WM_COMMAND_CMD(wParam, lParam), GET_WM_COMMAND_HWND(wParam, lParam));

	return Disp_Default(pcmi->dwp, hWnd, WM_COMMAND, wParam, lParam);
}

TOLCDECL LRESULT TOLAPI Disp_Notify(PT_NTFMAPINF pnmi, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int i;
	UINT uNtf=((LPNMHDR)lParam)->code;

	for(i=0; i<pnmi->num; i++)
		if(pnmi->pnm[i].uNtf==uNtf)
			return pnmi->pnm[i].pFn(hWnd, wParam, lParam);

	return Disp_Default(pnmi->dwp, hWnd, WM_NOTIFY, wParam, lParam);
}

TOLCDECL void TOLAPI Disp_State(PT_STTMAPINF psmi, HWND hWnd, HMENU hMenu, HWND hWndToolbar, WPARAM wParam, LPARAM lParam)
{
	int i;
	UINT uID;
	MENUITEMINFO mii;

	if(!hMenu && !hWndToolbar)
		return;

	mii.cbSize=sizeof(MENUITEMINFO);
	mii.fMask=MIIM_STATE;

	if(wParam>0 && lParam>0)
	{
		LPUINT pu=(LPUINT)lParam;

		for(wParam--; wParam>=0; wParam--)
		{
			uID=pu[wParam];

			for(i=0; i<psmi->num; i++)
				if(uID==psmi->psm[i].uID)
					if(psmi->psm[i].pFnRange)
					{
						UINT uMaxID=uID+psmi->psm[i].pFnRange();

						for(; uID<uMaxID; uID++)
						{
							psmi->psm[i].pFn(hWnd, uID, &mii);

							if(hMenu)
								SetMenuItemInfo(hMenu, uID, FALSE, &mii);

							if(hWndToolbar)
							{
								SendMessage(hWndToolbar, TB_CHECKBUTTON, uID, (mii.fState&MFS_CHECKED));
								SendMessage(hWndToolbar, TB_ENABLEBUTTON, uID, !(mii.fState&MFS_GRAYED));
							}
						}
					}
					else
					{
						psmi->psm[i].pFn(hWnd, uID, &mii);

						if(hMenu)
							SetMenuItemInfo(hMenu, uID, FALSE, &mii);

						if(hWndToolbar)
						{
							SendMessage(hWndToolbar, TB_CHECKBUTTON, uID, (mii.fState&MFS_CHECKED));
							SendMessage(hWndToolbar, TB_ENABLEBUTTON, uID, !(mii.fState&MFS_GRAYED));
						}
					}
		}
	}
	else if(wParam==0 && lParam>0)
	{
		uID=(UINT)lParam;

		for(i=0; i<psmi->num; i++)
			if(uID==psmi->psm[i].uID)
			{
				if(psmi->psm[i].pFnRange)
				{
					UINT uMaxID=uID+psmi->psm[i].pFnRange();

					for(; uID<uMaxID; uID++)
					{
						psmi->psm[i].pFn(hWnd, uID, &mii);

						if(hMenu)
							SetMenuItemInfo(hMenu, uID, FALSE, &mii);

						if(hWndToolbar)
						{
							SendMessage(hWndToolbar, TB_CHECKBUTTON, uID, (mii.fState&MFS_CHECKED));
							SendMessage(hWndToolbar, TB_ENABLEBUTTON, uID, !(mii.fState&MFS_GRAYED));
						}
					}
				}
				else
				{
					psmi->psm[i].pFn(hWnd, uID, &mii);

					if(hMenu)
						SetMenuItemInfo(hMenu, uID, FALSE, &mii);

					if(hWndToolbar)
					{
						SendMessage(hWndToolbar, TB_CHECKBUTTON, uID, (mii.fState&MFS_CHECKED));
						SendMessage(hWndToolbar, TB_ENABLEBUTTON, uID, !(mii.fState&MFS_GRAYED));
					}
				}

				break;
			}
	}
	else
	{
		for(i=0; i<psmi->num; i++)
		{
			uID=psmi->psm[i].uID;

			if(psmi->psm[i].pFnRange)
			{
				UINT uMaxID=uID+psmi->psm[i].pFnRange();

				for(; uID<uMaxID; uID++)
				{
					psmi->psm[i].pFn(hWnd, uID, &mii);

					if(hMenu)
						SetMenuItemInfo(hMenu, uID, FALSE, &mii);

					if(hWndToolbar)
					{
						SendMessage(hWndToolbar, TB_CHECKBUTTON, uID, (mii.fState&MFS_CHECKED));
						SendMessage(hWndToolbar, TB_ENABLEBUTTON, uID, !(mii.fState&MFS_GRAYED));
					}
				}
			}
			else
			{
				psmi->psm[i].pFn(hWnd, uID, &mii);

				if(hMenu)
					SetMenuItemInfo(hMenu, uID, FALSE, &mii);

				if(hWndToolbar)
				{
					SendMessage(hWndToolbar, TB_CHECKBUTTON, uID, (mii.fState&MFS_CHECKED));
					SendMessage(hWndToolbar, TB_ENABLEBUTTON, uID, !(mii.fState&MFS_GRAYED));
				}
			}
		}
	}
}


TOLEXTC_END
