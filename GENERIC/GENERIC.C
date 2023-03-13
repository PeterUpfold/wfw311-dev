#define STRICT

#include <windows.h>
#include "generic.h"

HINSTANCE hInst;

int PASCAL WinMain(hInstCurrent, hInstPrevious, lpszCmdLine, nCmdShow)
HINSTANCE hInstCurrent; // current instance
HINSTANCE hInstPrevious; // any existing instance if present and easy to do
LPSTR lpszCmdLine;
int nCmdShow;
{
	MSG msg;
	
	if (!hInstPrevious) {
		if (!InitApplication(hInstCurrent)) // init shared vars
			return FALSE;
	}
	
	// instance-specific init
	if (!InitInstance(hinstCurrent, nCmdShow))
		return FALSE;
		
	// message pump
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg); // scan codes to other stuff(tm)
		DispatchMessage(&msg);
	}
	
	return (msg.wParam);
	
}	

BOOL InitApplication(hInstCurrent)
HINSTANCE hInstCurrent;
{
	WNDCLASS wc;
	
	wc.style = NULL;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	
	wc.hInstance = hInstCurrent;
	wc.hIcon = LoadIcon(NULL, IDL_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = "GenericMenu"; // from .RC file
	wc.lpszClassName = "GenericWClass";
	
	return (RegisterClass(&wc));
	

}

BOOL InitInstance(hInstCurrent, nCmdShow)
HINSTANCE hInstCurrent;
int nCmdShow;
{
	HWND hWnd;
	
	hInst = hInstCurrent; // global??
	
	hWnd = CreateWindow(
		"GenericWClass",
		"Generic Sample Application",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstCurrent,
		NULL
	);
	
	if (NULL == hWnd) {
		return FALSE;
	}
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

LRESULT FAR PASCAL MainWndProc(hWnd, message, wParam, lParam);
HWND hWnd;
UINT message;
WPARAM wParam;
LPARAM lParam;
{
	FARPROC lpProcAbout; // pointer to about procedure
	switch (message) {
		case WM_COMMAND:
			// menu item
			if (wParam == IDM_ABOUT) {
				lpProcAbout = MakeProcInstance((FARPROC) About, hInst);
				
				DialogBox(hInst, "AboutBox", hWnd, (DLGPROC) lpProcAbout);
				FreeProcInstance(lpProcAbout);
				break;
			}
			return (DefWindowProc(hWnd, message, wParam, lParam);
           
		case WM_DESTROY:        // close main window
			PostQuitMessage(0);
			break;
			
		default:
			return (DefWindowProc(hWnd, message, wParam, lParam);
			
		}
		return NULL;
}

BOOL FAR PASCAL About(hDlg, message, wParam, lParam)
HWND hDlg;
WORD message;
WPARAM wParam;
LPARAM lParam;
{
	switch (message) {
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND:
			if (wParam == IDOK || wParam == IDCANCEL) {
				EndDialog(hDlg, TRUE);
				return TRUE;
			}
			break;
	}
	return FALSE;
}