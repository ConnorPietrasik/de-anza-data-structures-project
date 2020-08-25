#include<Windows.h>
#include"GUI/DisplayProductWindow.h"
#include"Product.h"

void DisplayProductWindow::Init() {
	CreateWindowW(
		L"ListBox",
		L"Product Info",
		WS_CHILD | WS_VISIBLE | LBS_NOSEL | WS_VSCROLL | WS_HSCROLL | WS_BORDER,		//LBS_NOSEL means you can't select the items because it's just display
		0, 0, 0, 0,
		m_hwnd,
		(HMENU)1,
		GetModuleHandleW(NULL),
		NULL
	);
}

void DisplayProductWindow::Paint() {
	PAINTSTRUCT ps = {};
	HDC hdc = BeginPaint(m_hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BACKGROUND));
	EndPaint(m_hwnd, &ps);
}

void DisplayProductWindow::Resize() {
	RECT size;
	GetClientRect(m_hwnd, &size);

	SetWindowPos(
		GetDlgItem(m_hwnd, 1),
		NULL,
		size.right / 20,
		size.bottom / 40,
		size.right - (2 * (size.right / 20)),
		size.bottom - (2 * (size.bottom / 50)),
		SWP_NOZORDER
	);
}

//Sets the product being displayed
void DisplayProductWindow::setProduct(Product* p) {
	SendMessageW(GetDlgItem(m_hwnd, 1), LB_RESETCONTENT, 0, 0);

	int longest = 0;

	//Displays all the string parameters, uses function pointers to avoid a giant block of copy/paste
	const std::wstring typeNames[] = { L"Name:          ", L"Desc:           ", L"UPC:            ", L"Sku:             ", L"Brand:         ", L"Model:         ", L"Dept:           ", L"SubDept:     " };
	const std::string& (Product:: * fns[])() const = { &Product::getName, &Product::getDesc, &Product::getUpc, &Product::getSku, &Product::getBrand, &Product::getModel, &Product::getDept, &Product::getSubDept };
	for (int i = 0; i < 8; i++) {
		std::wstring type = typeNames[i];
		std::wstring info((p->*fns[i])().begin(), (p->*fns[i])().end());
		SendMessageW(GetDlgItem(m_hwnd, 1), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((type + info).c_str()));
		if (info.length() > longest) longest = info.length();
	}

	//Displays the price to 2 decimal places
	std::wstring type = L"Price:          $";
	std::wstring info = std::to_wstring(p->getPrice());
	info = info.substr(0, info.find('.') + 3);
	SendMessageW(GetDlgItem(m_hwnd, 1), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((type + info).c_str()));
	if (info.length() > longest) longest = info.length();

	type = L"Stock:          ";
	info = std::to_wstring(p->getStock());
	SendMessageW(GetDlgItem(m_hwnd, 1), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((type + info).c_str()));
	if (info.length() > longest) longest = info.length();

	type = L"# Storage:   ";
	info = std::to_wstring(p->getWare());
	SendMessageW(GetDlgItem(m_hwnd, 1), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>((type + info).c_str()));
	if (info.length() > longest) longest = info.length();

	//Sets the display window size to fit the text no matter the length (within reason)
	SetWindowPos(
		m_hwnd,
		NULL,
		0, 0, 90 + longest * 10, 250,
		SWP_NOZORDER | SWP_NOMOVE
	);
}


LRESULT DisplayProductWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_CREATE:
		Init();
		return 0;

	case WM_PAINT:
		Paint();
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