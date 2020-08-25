#include"GUI/TextOutputWindow.h"
#include<Windows.h>


void TextOutputWindow::Init() {
	SetWindowPos(
		m_hwnd,
		NULL,
		0, 0,
		800, 500,
		SWP_NOZORDER | SWP_NOMOVE
	);

	CreateWindowW(
		L"Edit",
		L"Output",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL,
		0, 0, 0, 0,
		m_hwnd,
		(HMENU)1,
		GetModuleHandleW(NULL),
		NULL
	);
}

void TextOutputWindow::Resize() {
	RECT size;
	GetClientRect(m_hwnd, &size);
	SetWindowPos(		//Sets the textbox to fill the whole window
		GetDlgItem(m_hwnd, 1),
		NULL,
		0,
		0,
		size.right,
		size.bottom,
		SWP_NOZORDER
	);
}

LRESULT TextOutputWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_CREATE:
		Init(); 
		return 0;

	case WM_SIZE:
		Resize();
		return 0;

	case WM_CLOSE:
		ShowWindow(m_hwnd, 0);
		return 0;

	default:
		return DefWindowProcW(m_hwnd, msg, wParam, lParam);
	}
}