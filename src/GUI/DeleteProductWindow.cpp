#include<Windows.h>
#include"GUI/DeleteProductWindow.h"
#include<string>

void DeleteProductWindow::Init() {
	CreateWindowW(
		L"EDIT",
		L"",
		WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE,
		10, 10, 214, 20,
		m_hwnd,
		(HMENU)1,
		GetModuleHandleW(NULL),
		NULL
	);

	CreateWindowW(
		L"BUTTON",
		L"Delete Product",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		30, 50, 175, 30,
		m_hwnd,
		(HMENU)2,
		GetModuleHandleW(NULL),
		NULL
	);
}

void DeleteProductWindow::Paint() {
	PAINTSTRUCT ps = {};
	HDC hdc = BeginPaint(m_hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, (HBRUSH)COLOR_BACKGROUND);
	EndPaint(m_hwnd, &ps);
}

void DeleteProductWindow::OnDelete() {
	int len = GetWindowTextLengthW(GetDlgItem(m_hwnd, 1)) + 1;
	wchar_t* input = new wchar_t[len];
	GetWindowTextW(GetDlgItem(m_hwnd, 1), input, len);
	std::wstring nameW(input);
	delete[] input;
	
	std::string name(nameW.begin(), nameW.end());

	std::wstring result = L"Product with name \"" + nameW + L"\" ";
	if (true) result += L"was successfully deleted.";
	else result += L"was not found.";

	MessageBoxW(m_hwnd, result.c_str(), L"Result", MB_OK);
}

LRESULT DeleteProductWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_CREATE:
		Init();
		return 0;

	case WM_PAINT:
		Paint();
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2: OnDelete(); break;
		}
		return 0;

	default:
		return DefWindowProcW(m_hwnd, msg, wParam, lParam);
	}
}