#include<Windows.h>
#include"GUI/AddProductWindow.h"
#include<string>
#include<sstream>
#include"Product.h"
#include"BST.h"
#include"Catalog.h"

//Sets everything up
void AddProductWindow::Init() {
	//Each of the text boxes that accept input, the default text gets set whenever the window is shown
	for (int i = 1; i < 12; i++) {
		CreateWindowW(
			L"Edit",
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			0, 0, 0, 0,
			m_hwnd,
			(HMENU)(i),
			GetModuleHandleW(NULL),
			NULL
		);
	}

	CreateWindowW(
		L"BUTTON",
		L"Add Product",
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		0, 0, 0, 0,
		m_hwnd,
		(HMENU)12,
		GetModuleHandleW(NULL),
		NULL
	);

	disp.Create(L"Product successfully added", 0, 0, GetSystemMetrics(SM_CXSCREEN) / 3, GetSystemMetrics(SM_CYSCREEN) / 5, m_hwnd);
}

//Fills the background of the window with basic background color
void AddProductWindow::Paint() {
	PAINTSTRUCT ps = {};
	HDC hdc = BeginPaint(m_hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, (HBRUSH)COLOR_BACKGROUND);
	EndPaint(m_hwnd, &ps);
}

//Resizes the window
void AddProductWindow::Resize() {

	//Gets the size of the client area (everything but title bar)
	RECT size;
	GetClientRect(m_hwnd, &size);

	long height = 20;
	long leftX = (size.right / 40);
	long rightX = (size.right / 2);
	long gap = (size.bottom / 20);
	long leftWidth = (size.right / 2) - (size.right / 20);
	long rightWidth = (size.right / 2) - (size.right / 40);

	//Puts all the text boxes in order
	for (int i = 1, j = 1; i < 12; i++) {
		if (i % 2) {
			SetWindowPos(
				GetDlgItem(m_hwnd, i),
				NULL,
				leftX,
				(j * gap) + ((j - 1) * height),
				leftWidth,
				height,
				SWP_NOZORDER
			);
		}
		else {
			SetWindowPos(
				GetDlgItem(m_hwnd, i),
				NULL,
				rightX,
				(j * gap) + ((j - 1) * height),
				rightWidth,
				height,
				SWP_NOZORDER
			);
			j++;
		}
	}

	//The add button, on its own because it has a different height
	SetWindowPos(
		GetDlgItem(m_hwnd, 12),
		NULL,
		rightX,
		((6 * gap) + (5 * height)),
		rightWidth,
		50,
		SWP_NOZORDER
	);
}

//Validates input, and adds the product
void AddProductWindow::OnAddButton() {
	std::string data[11];

	//Reads all the data in from the boxes
	for (int i = 1; i < 12; i++) {
		std::wstring inW = GetWindowTitle(GetDlgItem(m_hwnd, i));
		data[i - 1] = std::string(inW.begin(), inW.end());
	}

	if (data[0] == "" || data[0] == "*Name*") {
		MessageBoxW(m_hwnd, L"Must input a name", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	std::stringstream ss(data[8]);
	float price;
	if (data[8] == "" || data[8] == "*Price*") {
		MessageBoxW(m_hwnd, L"Must input a price", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}
	else if (!(ss >> price)) {
		MessageBoxW(m_hwnd, L"Must input a valid price (no $)", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	ss.ignore(100);
	ss.clear();
	ss << data[9];
	int stock;
	if (!(ss >> stock)) stock = 1;

	ss.ignore(100);
	ss.clear();
	ss << data[10];
	int wareQuant;
	if (!(ss >> wareQuant)) wareQuant = 0;

	//If another product has the same name, say no
	if (cat->getBST()->search(data[0], [](std::string n, Product* p) { return (n == p->getName()); }, [](std::string n, Product* p) { return (n >= p->getName()); })) {
		std::wstring out(data[0].begin(), data[0].end());
		out = L"Product with name \"" + out + L"\" already exists in database";
		MessageBoxW(m_hwnd, out.c_str(), L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	Product* prod = new Product(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], price, stock, wareQuant);
	cat->add(prod);

	disp.setProduct(prod);
	ShowWindow(disp.Window(), 1);
}


LRESULT AddProductWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
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

	case WM_COMMAND:
		if (LOWORD(wParam) == 12) OnAddButton();
		return 0;

	case WM_SHOWWINDOW:	//When the window is shown / hidden
	{
		if (wParam) {	//Only when the window is shown, resets the text boxes
			const wchar_t* labels[] = { L"*Name*", L"(Description)", L"(UPC)" , L"(SKU)" , L"(Brand)" , L"(Model)" , L"(Department)" , L"(Sub-Department)" , L"*Price*" , L"(Stock)" , L"(Warehouse Quantity)" };
			for (int i = 1; i < 12; i++) {
				SetWindowTextW(GetDlgItem(m_hwnd, i), labels[i - 1]);
			}
		}
		return 0;
	}

	case WM_CLOSE:
		ShowWindow(m_hwnd, 0);	//Don't actually close, just hide
		return 0;

	default:
		return DefWindowProcW(m_hwnd, msg, wParam, lParam);
	}
}