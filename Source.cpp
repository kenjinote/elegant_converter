#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <algorithm>
#include <iostream>

using namespace std;

TCHAR szClassName[] = TEXT("Window");

int Factor(int n, LPWSTR lpszText)
{
	if (n == 0) {
		return 0;
	}
	if (n == 1) {
		if (lpszText) {
			lstrcat(lpszText, L"+");
		}
		return 1;
	} else if (n % 2 == 0) {
		if (lpszText) {
			lstrcat(lpszText, L"D");
		}
		return 1 + Factor(n / 2, lpszText);
	} else {
		int plus = Factor(n + 1, 0);
		int minus = Factor(n - 1, 0);
		if (plus < minus) {
			if (lpszText) {
				lstrcat(lpszText, L"-");
			}
			return 1 + Factor(n + 1, lpszText);
		}
		else {
			if (lpszText) {
				lstrcat(lpszText, L"+");
			}
			return 1 + Factor(n - 1, lpszText);
		}
	}
}

UINT Calculate(LPCWSTR lpszText)
{
	UINT uNumber = 0;
	int nSize = lstrlen(lpszText);
	for (int i = 0;i < nSize;i++) {
		switch (lpszText[i]) {
		case L'+':
			uNumber++;
			break;
		case L'-':
			uNumber--;
			break;
		case L'D':
			uNumber<<=1;
			break;
		}
	}
	return uNumber;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton1;
	static HWND hButton2;
	static HWND hEdit1;
	static HWND hEdit2;
	switch (msg) {
	case WM_CREATE:
		hEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"349", WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER, 0, 0, 0, 0, hWnd, (HMENU)1000, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hButton1 = CreateWindow(L"BUTTON", L"変換↓", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hButton2 = CreateWindow(L"BUTTON", L"変換↑", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, hWnd, (HMENU)1002, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", 0, WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, (HMENU)1001, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_SIZE:
		MoveWindow(hEdit1, 10, 10, 512, 32, TRUE);
		MoveWindow(hButton1, 10, 50, 251, 32, TRUE);
		MoveWindow(hButton2, 271, 50, 251, 32, TRUE);
		MoveWindow(hEdit2, 10, 90, 512, 32, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			DWORD dwNumber = GetDlgItemInt(hWnd, 1000, 0, 0);
			WCHAR szOperatorList[1024] = {};
			Factor(dwNumber, szOperatorList);
			wstring s = szOperatorList;
			reverse(s.begin(), s.end());
			SetWindowText(hEdit2, s.c_str());
		} else if (LOWORD(wParam) == 1002) {
			WCHAR szText[1024];
			GetWindowText(hEdit2, szText, _countof(szText));
			UINT dwNumber = Calculate(szText);
			SetDlgItemInt(hWnd, 1000, dwNumber, FALSE);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;
	WNDCLASS wndclass = {
		0,
		WndProc,
		0,
		DLGWINDOWEXTRA,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		0,
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	RECT rect = { 0, 0, 532, 132 };
	AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU, FALSE);
	HWND hWnd = CreateWindow(
		szClassName,
		L"エレガントな変換機",
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		0,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0)) {
		if (!IsDialogMessage(hWnd, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
