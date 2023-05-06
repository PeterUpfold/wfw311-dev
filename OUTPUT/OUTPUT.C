#define STRICT

#include <windows.h>
#include "output.h"

HINSTANCE hInst;

// GDI variables
HPEN hDashPen; // "---" pen
HPEN hDotPen; // ... pen
HBRUSH hOldBrush;
HBRUSH hRedBrush;
HBRUSH hGreenBrush;
HBRUSH hBlueBrush;

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
	if (!InitInstance(hInstCurrent, nCmdShow))
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
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
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

LRESULT FAR PASCAL MainWndProc(hWnd, message, wParam, lParam)
HWND hWnd;
UINT message;
WPARAM wParam;
LPARAM lParam;
{
	FARPROC lpProcAbout; // pointer to about procedure
	
	HDC hDC; // device context
	PAINTSTRUCT ps; // paint structure
	RECT rcTextBox; // rectangle around the text box
	HPEN hOldPen; // old pen
	
	switch (message) {
		case WM_COMMAND:
			// menu item
			if (wParam == IDM_ABOUT) {
				lpProcAbout = MakeProcInstance((FARPROC) About, hInst);
				
				DialogBox(hInst, "AboutBox", hWnd, (DLGPROC) lpProcAbout);
				FreeProcInstance(lpProcAbout);
				break;
			}
			return (DefWindowProc(hWnd, message, wParam, lParam));
           
		case WM_DESTROY:        // close main window
			PostQuitMessage(0);
			break;          
			
		case WM_CREATE:
			// create brush objects
			hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
			hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
			hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
			
			// create dash pen
			hDashPen = CreatePen(PS_DASH, 1 /*width*/, RGB(0,0,0));
			
			// create dot pen
			hDotPen = CreatePen(2 /* dotted style??? */, 1 /*width*/, RGB(0,0,0));
		break;
		
		case WM_PAINT:
			{ /* block to scope these variables on the stack? */
				TEXTMETRIC tm;
				int nDrawX;
				int nDrawY;
				char szText[500];
				
				hDC = BeginPaint(hWnd, &ps);
				
				// get size characteristics of current font
				GetTextMetrics(hDC, &tm);
				
				// 1/4 inch from top and left of upper-left corner
				nDrawX = GetDeviceCaps(hDC, LOGPIXELSX) / 4;
				nDrawY = GetDeviceCaps(hDC, LOGPIXELSY) / 4;
				
				// paint some text
				lstrcpy(szText, "Paint some characters and stuff");
				TextOut(hDC, nDrawX, nDrawY, szText, lstrlen(szText));
				nDrawY += tm.tmExternalLeading + tm.tmHeight; // one line of metrics?
				
				lstrcpy(szText, "and another line");
				TextOut(hDC, nDrawX, nDrawY, szText, lstrlen(szText));
				
				// in a rect        -- not visible in terms of having a pen,
				// but helps us to not have to mess about manually with formatting
				nDrawY += GetDeviceCaps(hDC, LOGPIXELSY) / 4;
				SetRect(&rcTextBox,
					nDrawX,
					nDrawY,
					nDrawX + (5 * GetDeviceCaps(hDC, LOGPIXELSY)),
					nDrawY + (1 * GetDeviceCaps(hDC, LOGPIXELSX))
				);
				
				
				// draw text in rect
				lstrcpy(szText, "This is a really long string, but I shouldn't have to worry at"
				"all about line break and word wrap and such disappointing implementation details"
				"that should not concern the likes of me");
				
				DrawText(hDC, szText, lstrlen(szText), &rcTextBox,
					DT_CENTER | DT_EXTERNALLEADING | DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK);
					
				nDrawY = rcTextBox.bottom;
				
				hOldBrush = SelectObject(hDC, hRedBrush);                
				// red GDI rect
				Rectangle(hDC, nDrawX, nDrawY, nDrawX + 50, nDrawY + 30);
				
				SelectObject(hDC, hGreenBrush);
				Ellipse(hDC,nDrawX + 150, nDrawY, nDrawX + 150 + 50, nDrawY + 30);
				
				// blue pie. What flavour? Pie flavour. Waaaaah
				SelectObject(hDC, hBlueBrush);
				Pie(hDC, nDrawX + 300, nDrawY, nDrawX + 300 + 50,
				nDrawY + 50,
				nDrawX + 300 + 50,
				nDrawY,
				nDrawX + 300 + 50,
				nDrawY + 50);
				
				nDrawY += 50;
				
				// restore original brush
				SelectObject(hDC, hOldBrush);
				
				
				
				
				
			}
		break;
			
		default:
			return (DefWindowProc(hWnd, message, wParam, lParam));
			
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