#include<Windows.h>
#include"GUI/FindProductWindow.h"
#include<string>
#include"BST.h"
#include"Product.h"
#include"TArray.h"
#include"Catalog.h"

void FindProductWindow::Init() {
	delMode = false;

	CreateWindowW(
		L"EDIT",
		L"Name",
		WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE,
		10, 10, 214, 20,
		m_hwnd,
		(HMENU)1,
		GetModuleHandleW(NULL),
		NULL
	);

	CreateWindowW(
		L"BUTTON",
		L"Find Product by Name",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		30, 40, 175, 30,
		m_hwnd,
		(HMENU)2,
		GetModuleHandleW(NULL),
		NULL
	);

	CreateWindowW(
		L"EDIT",
		L"UPC",
		WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE,
		10, 110, 214, 20,
		m_hwnd,
		(HMENU)3,
		GetModuleHandleW(NULL),
		NULL
	);

	CreateWindowW(
		L"BUTTON",
		L"Find Product by UPC",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		30, 140, 175, 30,
		m_hwnd,
		(HMENU)4,
		GetModuleHandleW(NULL),
		NULL
	);

	disp.Create(L"Found:", 400, 500, GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 3, m_hwnd, WS_OVERLAPPEDWINDOW | WS_POPUP, WS_EX_DLGMODALFRAME);
}

//Fills the background of the window with basic background color
void FindProductWindow::Paint() {
	PAINTSTRUCT ps = {};
	HDC hdc = BeginPaint(m_hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, (HBRUSH)COLOR_BACKGROUND);
	EndPaint(m_hwnd, &ps);
}

//Searches by name
void FindProductWindow::OnName() {
	//GetDlgItem gets a handle to the specified control, the second parameter is the control's ID
	std::wstring nameW = GetWindowTitle(GetDlgItem(m_hwnd, 1));

	//A basic way to convert wstring to string, it doesn't work for non-english characters but good enough for this purpose
	std::string name(nameW.begin(), nameW.end());
	TArray<Product*, CompareName, EqualityName, PrintName> results;

	//If the name length is greater than 12 characters the similarly named items are probably gone, so it does the basic BST search (otherwise results get cut)
	if (name.length() < 12) results = cat->getBST()->findAllBrute(name, [&](std::string n, Product* p) { return (p->getName().find(n) != std::string::npos); });
	else results = cat->getBST()->findAll(name, [&](std::string n, Product* p) { return (p->getName().find(n) != std::string::npos); }, [&](std::string n, Product* p) { return (n >= p->getName()); });
	
	if (results.size() > 0) {
		disp.FillList(results);
		ShowWindow(disp.Window(), 1);
	}
	else {
		nameW = L"No product with a name containing \"" + nameW + L"\" was found.";
		MessageBoxW(m_hwnd, nameW.c_str(), L"PRODUCT NOT FOUND", MB_OK);
	}
}

//Searches by UPC
void FindProductWindow::OnUPC() {
	std::wstring UPCW = GetWindowTitle(GetDlgItem(m_hwnd, 3));
	std::string UPC(UPCW.begin(), UPCW.end());
	TArray<Product*, CompareName, EqualityName, PrintName> results;
	auto temp = cat->getHash()->find(UPC);
	if (temp) results.push_back(temp);

	if (results.size() > 0) {
		disp.FillList(results);
		ShowWindow(disp.Window(), 1);
	}
	else {
		UPCW = L"No product with a UPC of \"" + UPCW + L"\" was found.";
		MessageBoxW(m_hwnd, UPCW.c_str(), L"PRODUCT NOT FOUND", MB_OK);
	}
}

//Sets the window to delete mode or find mode, because they do basically the same thing
void FindProductWindow::SetDelMode(bool x) {
	SetWindowTextW(GetDlgItem(m_hwnd, 1), L"*Insert Name*");
	delMode = x;
	if (x) disp.SetMode(ProductListWindow::DELMODE);
	else disp.SetMode(ProductListWindow::VIEWMODE);
	if (x) {
		SetWindowTextW(m_hwnd, L"Delete Product");
		SetWindowTextW(GetDlgItem(m_hwnd, 2), L"Find Product for Deletion");
	}
	else {
		SetWindowTextW(m_hwnd, L"Find Product");
		SetWindowTextW(GetDlgItem(m_hwnd, 2), L"Find Product");
	}
}

LRESULT FindProductWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_CREATE:
		Init();
		return 0;

	case WM_PAINT:
		Paint();
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2: OnName(); break;
		case 4: OnUPC(); break;
		}
		return 0;

	case WM_SHOWWINDOW:
		SetWindowTextW(GetDlgItem(m_hwnd, 1), L"*Insert Name*");
		SetWindowTextW(GetDlgItem(m_hwnd, 3), L"*Insert UPC*");
		return 0;

	case WM_CLOSE:
		ShowWindow(m_hwnd, 0);
		return 0;

	default:
		return DefWindowProcW(m_hwnd, msg, wParam, lParam);
	}
}