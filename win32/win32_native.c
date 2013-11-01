
#include <windows.h>
#include <winuser.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <cairo.h>
#include <cairo-win32.h>

#include "emulator.h"
#include "native_graphic.h"
#include "emu-shell.h"

#define EMULATOR_BUTTON_UP			1001
#define EMULATOR_BUTTON_DOWN		1002
#define EMULATOR_BUTTON_ENTER		1003
#define EMULATOR_BUTTON_ESCAPE		1004
	
#define EMULATOR_BUTTON_ENTERUP		2001
#define EMULATOR_BUTTON_ENTERDOWN	2002
#define EMULATOR_BUTTON_ESCAPEUP	2003
#define EMULATOR_BUTTON_ESCAPEDOWN	2004

#define EMULATOR_BUTTON_MESSAGE		3001
#define EMULATOR_BUTTON_CALL		3002

// Global variables

// The main window class name.
const static TCHAR szWindowClass[] = _T("smatch_emu");

// The string that appears in the application's title bar.
const static TCHAR szTitle[] = _T("Smatch Emulator");

const static TCHAR szIcon[]  = _T("smatch-emu.ico");

// for logger out edit
static HWND	__gs_logger_win = NULL;

static void SetLogger(HWND hEdit){
	__gs_logger_win = hEdit;
}

static HWND GetLogger(){
	return __gs_logger_win;
}

static HWND	gs_hWnd = NULL;

static void AppendText(HWND hWnd, LPCTSTR lpstrText);

int logger_output(const char *log, int size){
	HWND	logWnd = GetLogger();

	if(logWnd != NULL){
		AppendText(logWnd, log);
		AppendText(logWnd, "\r\n");

		return size;
	}else{
		return -1;
	}
}

// Forward declarations of functions included in this code module:
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//int main(int argc, const char *argv)
int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmd, int nCmd)
{
    WNDCLASSEX	wcex;
	HWND		hWnd;
	MSG			msg;

    wcex.cbSize			= sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInst;
    wcex.hIcon          = LoadIcon(hInst, szIcon);
    wcex.hCursor        = NULL;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(hInst, szIcon);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, _T("Register Emulator ClassEx failed!"), szTitle, 0);

        return 1;
    }

    hWnd = CreateWindow(szWindowClass, szTitle,
        WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX, 
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 400,
        NULL, NULL, hInst, NULL);

    if (!hWnd)
    {
        MessageBox(NULL, _T("CreateWindow failed!"), szTitle, 0);

        return 1;
    }

	gs_hWnd = hWnd;

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    // Main message loop:
    while(1)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

//  DestroyWindow(hWnd);
    UnregisterClass(szWindowClass, hInst);

    return 0;
}

static HWND CreateButton(HWND hWnd, LPTSTR lpText, RECT rc, int id){
	HWND	button;

	button = CreateWindow(_T("BUTTON"), lpText,
			WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON|BS_NOTIFY,
			rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, hWnd, (HMENU)id, NULL, NULL);

	return button;
}

static void AppendText(HWND hWnd, LPCTSTR lpstrText){
	DWORD l,r;
	
//	SendMessage(hWnd, EM_GETSEL,(WPARAM)&l,(LPARAM)&r);
	SendMessage(hWnd, EM_SETSEL, -1, -1);
	SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)lpstrText);
//	SendMessage(hWnd, EM_SETSEL,l,r);
}


static void OnWindowCreate(HWND hWnd){
	RECT	rc;
	HWND	hEdit;

	// create edit for logger
	hEdit = CreateWindowEx(0, _T("EDIT"), NULL,
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_BORDER
			|ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|ES_READONLY,
			520, 0, 480, 450,
			hWnd, NULL, NULL, NULL);
	if(hEdit != NULL){
		SetLogger(hEdit);
		AppendText(hEdit,(LPARAM)_T("..........emulator.......\r\n"));
	}

	// create buttons
	rc.top		= 30;
	rc.bottom	= 60;
	rc.left		= 280;
	rc.right	= 380;
	CreateButton(hWnd, _T("Enter"), rc, EMULATOR_BUTTON_ENTER);
	
	rc.top		= 70;
	rc.bottom	= 100;
	CreateButton(hWnd, _T("Escape"), rc, EMULATOR_BUTTON_ESCAPE);


	rc.top		= 30;
	rc.bottom	= 60;
	rc.left		= 390;
	rc.right	= 490;
	CreateButton(hWnd, _T("Up"), rc, EMULATOR_BUTTON_UP);

	
	rc.top		= 70;
	rc.bottom	= 100;
	CreateButton(hWnd, _T("Down"), rc, EMULATOR_BUTTON_DOWN);

	
	rc.top		= 190;
	rc.bottom	= 220;
	rc.left		= 280;
	rc.right	= 380;
	CreateButton(hWnd, _T("EnterUp"), rc, EMULATOR_BUTTON_ENTERUP);

	
	rc.top		= 230;
	rc.bottom	= 260;
	CreateButton(hWnd, _T("EnterDown"), rc, EMULATOR_BUTTON_ENTERDOWN);

	
	rc.top		= 190;
	rc.bottom	= 220;
	rc.left		= 390;
	rc.right	= 490;
	CreateButton(hWnd, _T("EscUp"), rc, EMULATOR_BUTTON_ESCAPEUP);

	
	rc.top		= 230;
	rc.bottom	= 260;
	CreateButton(hWnd, _T("EscDown"), rc, EMULATOR_BUTTON_ESCAPEDOWN);

	
	rc.top		= 360;
	rc.bottom	= 390;
	rc.left		= 280;
	rc.right	= 380;
	CreateButton(hWnd, _T("Message"), rc, EMULATOR_BUTTON_MESSAGE);

	
	rc.left		= 390;
	rc.right	= 490;
	CreateButton(hWnd, _T("Call"), rc, EMULATOR_BUTTON_CALL);

	return ;
}

static void ClientResize(HWND hWnd, int nWidth, int nHeight)

{
	RECT rcClient, rcWindow;
	POINT ptDiff;

	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);

	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(hWnd,rcWindow.left, rcWindow.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}

static void OnCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	WORD	id, opcode;

	id = LOWORD(wParam);
	opcode = HIWORD(wParam);
	HWND	log;

	switch(opcode){
	case BN_CLICKED:
		log = GetLogger();
		if(log != NULL){
			id = 0;
			AppendText(log, _T("EMU : key clicked\r\n"));
		}
		break;

	default:
        DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC			hdc;

    switch (msg)
    {
	case WM_CREATE:
		ClientResize(hWnd, 1000, 450);

		OnWindowCreate(hWnd);

		native_graphic_init();

		emulator_init();

		break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
		{
			// create surface of windows dc
			cairo_surface_t *surface = cairo_win32_surface_create(hdc);
            cairo_t			*cr = cairo_create(surface);

			emu_shell_t		*es = emu_shell_getinfo(0);
		
			// draw watch content
		{
			cairo_surface_t *ngsurface = native_graphic_get();
			cairo_set_source_surface(cr, ngsurface, es->es_screen_x, es->es_screen_y);
			cairo_paint(cr);
		}
            
			cairo_surface_finish(surface);
            
			// Cleanup.
            cairo_destroy(cr);
            cairo_surface_destroy(surface);
		}
		EndPaint(hWnd, &ps);
        break;

	case WM_ERASEBKGND:
		hdc = (HDC)wParam;
		{
			cairo_surface_t *surface = cairo_win32_surface_create(hdc);
			cairo_t			*cr = cairo_create(surface);
			RECT			rc;
			
			GetClientRect(hWnd, &rc);
	
			emu_shell_draw(cr, surface, rc.right - rc.left, rc.bottom - rc.top);

			cairo_surface_finish(surface);

			cairo_destroy(cr);
			cairo_surface_destroy(surface);
		}
		break;

	case WM_COMMAND:
		OnCommand(hWnd, msg, wParam, lParam);
		break;

	case WM_DESTROY:
		emulator_fini();

		native_graphic_fini();
        
		PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }

    return 0;
}

int native_flush_device(){
	RECT			rc;
	emu_shell_t		*es = emu_shell_getinfo(0);

	rc.top = es->es_screen_y;
	rc.left = es->es_screen_x;
	rc.bottom = rc.top + es->es_screen_height;
	rc.right  = rc.left + es->es_screen_width;
	
	InvalidateRect(gs_hWnd, &rc, FALSE);

	return 0;
}


