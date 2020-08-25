#include<Windows.h>
#include"GUI/ProductListWindow.h"
#include"GUI/DisplayProductWindow.h"

#include"BST.h"
#include"TArray.h"
#include<string>
#include<fstream>

void ProductListWindow::Init() {
	mode = VIEWMODE;

	CreateWindowW(
		L"ListBox",
		L"Product List",
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,		//LBS_NOTIFY means it tells the window if someone double clicks or something
		0, 0, 0, 0,
		m_hwnd,
		(HMENU)1,
		GetModuleHandleW(NULL),
		NULL
	);

	const wchar_t* lbls[] = { L"View Product", L"Delete Product" , L"Add to Cart", L"Remove from Cart", L"Checkout" };
	for (int i = 0; i < 5; i++) {
		CreateWindowW(
			L"BUTTON",
			lbls[i],
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			0, 0, 0, 0,
			m_hwnd,
			(HMENU)(i + 2),
			GetModuleHandleW(NULL),
			NULL
		);
	}
	
	disp.Create(L"Product Display", 0, 0, GetSystemMetrics(SM_CXSCREEN) / 3, GetSystemMetrics(SM_CYSCREEN) / 4, m_hwnd, WS_OVERLAPPEDWINDOW | WS_POPUP);
	textWin.Create(L"Output", 0, 0, GetSystemMetrics(SM_CXSCREEN) / 3, GetSystemMetrics(SM_CYSCREEN) / 4, m_hwnd, WS_OVERLAPPEDWINDOW | WS_POPUP, WS_EX_DLGMODALFRAME);

	//Sets the font of the receipt output window to a monospace font so stuff lines up (that's what FF_MODERN means basically, the rest is telling it to do default / English windows)
	font = CreateFontW(0, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, NULL);
	SendMessageW(GetDlgItem(textWin.Window(), 1), WM_SETFONT, (WPARAM)font, true);
}

void ProductListWindow::Paint() {
	PAINTSTRUCT ps = {};
	HDC hdc = BeginPaint(m_hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BACKGROUND));
	EndPaint(m_hwnd, &ps);
}

void ProductListWindow::Resize() {
	RECT size;
	GetClientRect(m_hwnd, &size);

	SetWindowPos(
		GetDlgItem(m_hwnd, 1),
		NULL,
		size.right / 40,
		size.bottom / 50,
		size.right - (2 * (size.right / 40)),
		size.bottom - (size.bottom / 4),
		SWP_NOZORDER
	);

	//Sets the buttons based on mode, for example there's no need for space for checkout if it's not visible
	if (mode == VIEWMODE) {
		SetWindowPos(
			GetDlgItem(m_hwnd, 2),
			NULL,
			size.right / 40,
			size.bottom - (size.bottom / 5),
			(size.right / 2) - (size.right / 20),
			size.bottom - (size.bottom - (size.bottom / 6)),
			SWP_NOZORDER
		);

		SetWindowPos(
			GetDlgItem(m_hwnd, 4),
			NULL,
			size.right / 2,
			size.bottom - (size.bottom / 5),
			(size.right / 2) - (size.right / 40),
			size.bottom - (size.bottom - (size.bottom / 6)),
			SWP_NOZORDER
		);
	}
	else if (mode == DELMODE) {
		SetWindowPos(
			GetDlgItem(m_hwnd, 2),
			NULL,
			size.right / 40,
			size.bottom - (size.bottom / 5),
			(size.right / 2) - (size.right / 20),
			size.bottom - (size.bottom - (size.bottom / 6)),
			SWP_NOZORDER
		);

		SetWindowPos(
			GetDlgItem(m_hwnd, 3),
			NULL,
			size.right / 2,
			size.bottom - (size.bottom / 5),
			(size.right / 2) - (size.right / 40),
			size.bottom - (size.bottom - (size.bottom / 6)),
			SWP_NOZORDER
		);
	}
	else if (mode == CARTMODE) {
		SetWindowPos(
			GetDlgItem(m_hwnd, 1),
			NULL,
			size.right / 40,
			size.bottom / 50,
			size.right - (2 * (size.right / 40)),
			size.bottom - (size.bottom / 2.5),
			SWP_NOZORDER
		);

		SetWindowPos(
			GetDlgItem(m_hwnd, 2),
			NULL,
			size.right / 40,
			size.bottom - (size.bottom / 2.75),
			(size.right / 2) - (size.right / 20),
			size.bottom - (size.bottom - (size.bottom / 6)),
			SWP_NOZORDER
		);

		SetWindowPos(
			GetDlgItem(m_hwnd, 5),
			NULL,
			size.right / 2,
			size.bottom - (size.bottom / 2.75),
			(size.right / 2) - (size.right / 40),
			size.bottom - (size.bottom - (size.bottom / 6)),
			SWP_NOZORDER
		);

		SetWindowPos(
			GetDlgItem(m_hwnd, 6),
			NULL,
			size.right / 40,
			size.bottom - (size.bottom / 5.5),
			size.right - size.right / 20,
			size.bottom - (size.bottom - (size.bottom / 6)),
			SWP_NOZORDER
		);
	}

}

//Sets the display mode, and makes sure only the proper buttons are visible
void ProductListWindow::SetMode(MODE x) {
	mode = x;
	if (mode == VIEWMODE) ShowWindow(GetDlgItem(m_hwnd, 4), 1);
	else ShowWindow(GetDlgItem(m_hwnd, 4), 0);
	if (mode == DELMODE) ShowWindow(GetDlgItem(m_hwnd, 3), 1);
	else ShowWindow(GetDlgItem(m_hwnd, 3), 0);
	if (mode == CARTMODE) {
		ShowWindow(GetDlgItem(m_hwnd, 5), 1);
		ShowWindow(GetDlgItem(m_hwnd, 6), 1);
	}
	else {
		ShowWindow(GetDlgItem(m_hwnd, 5), 0);
		ShowWindow(GetDlgItem(m_hwnd, 6), 0);
	}
	Resize();
}

//Fills the list of products with the given TArray
void ProductListWindow::FillList(TArray<Product*, CompareName, EqualityName, PrintName> arr) {
	SendMessageW(GetDlgItem(m_hwnd, 1), LB_RESETCONTENT, 0, 0);		//Tells the list to be empty
	curProds = arr;
	int longest = 0;
	for (auto p : arr) {
		std::wstring wName(p->getName().begin(), p->getName().end());
		if (wName.length() > longest) longest = wName.length();
		SendMessageW(GetDlgItem(m_hwnd, 1), LB_ADDSTRING, NULL, reinterpret_cast<LPARAM>(wName.c_str()));	//Adds the product's name to the list
	}

	//Make sure the window is big enough to display everything
	if (longest > 25) SetWindowPos(
		m_hwnd,
		NULL, 0, 0,
		longest * 10,
		500,
		SWP_NOZORDER | SWP_NOMOVE
	);
	else SetWindowPos(
		m_hwnd,
		NULL, 0, 0,
		300,
		500,
		SWP_NOZORDER | SWP_NOMOVE
	);
	
	//Put the total price at the top (could've made it it's own area, but focusing on other stuff instead)
	if (mode == CARTMODE) {
		std::wstring tot = L"Total price (pre-tax): $";
		tot += std::to_wstring(cat->getCart()->getCost());
		tot = tot.substr(0, tot.find('.') + 3);
		SendMessageW(GetDlgItem(m_hwnd, 1), LB_ADDSTRING, NULL, reinterpret_cast<LPARAM>(tot.c_str()));
	}
}

//Displays the selected product
void ProductListWindow::OnViewPushed() {
	int index = SendMessageW(GetDlgItem(m_hwnd, 1), LB_GETCURSEL, 0, 0);		//Asks the listbox what item is selected
	if (index != -1) {	//-1 means nothing is selected
		if (mode != CARTMODE || index != curProds.size()) {		//If it's displaying a cart, you can't view the price because it's not a product
			std::wstring temp(curProds[index]->getName().begin(), curProds[index]->getName().end());
			SetWindowTextW(disp.Window(), temp.c_str());
			disp.setProduct(curProds[index]);
			ShowWindow(disp.Window(), 1);
		}
	}
}

//Deletes the selected product
void ProductListWindow::OnDelPushed() {
	int index = SendMessageW(GetDlgItem(m_hwnd, 1), LB_GETCURSEL, 0, 0);
	if (index > -1) {
		std::wstring name(curProds[index]->getName().begin(), curProds[index]->getName().end());
		cat->remove(curProds[index]);
		curProds.erase(index);		//Removes it from the products currently displayed as well
		name = L"Product named \"" + name + L"\" successfully deleted.";
		MessageBoxW(m_hwnd, name.c_str(), L"Success", MB_OK);	//Messagebox notifying of success, gets pretty annoying if deleting multiple things but oh well
		FillList(curProds);
	}
}

//Adds the product to the cart
void ProductListWindow::OnAddPushed() {
	int index = SendMessageW(GetDlgItem(m_hwnd, 1), LB_GETCURSEL, 0, 0);
	if (index != -1) {
		cat->getCart()->add(curProds[index]);
		MessageBoxW(m_hwnd, L"Product successfully added to cart", L"Success!", MB_OK);
	}
}

//Removes the product from the cart
void ProductListWindow::OnRemovePushed() {
	int index = SendMessageW(GetDlgItem(m_hwnd, 1), LB_GETCURSEL, 0, 0);
	if (index > -1 && index != curProds.size()) {
		std::wstring name(curProds[index]->getName().begin(), curProds[index]->getName().end());
		cat->getCart()->remove(curProds[index]);
		curProds.erase(index);
		FillList(curProds);
	}
}

//Prints the receipt and subtracts the stock
void ProductListWindow::OnCheckoutPushed() {
	std::string receiptA = cat->getCart()->reciept();
	std::wstring receiptW = ConvertToWString(receiptA);

	SetWindowTextW(GetDlgItem(textWin.Window(), 1), receiptW.c_str());
	SetWindowTextW(textWin.Window(), L"Tree");
	ShowWindow(textWin.Window(), 1);

	auto prods = cat->getCart()->getCart();
	for (auto p : prods) {
		p->sell();
	}

	cat->getCart()->clear();	//Empties the cart
	FillList();					//Clears the list
}


LRESULT ProductListWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
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
		switch (LOWORD(wParam)) {
		case 1: if (HIWORD(wParam) == LBN_DBLCLK) OnViewPushed(); break;
		case 2: OnViewPushed(); break;
		case 3: OnDelPushed(); break;
		case 4: OnAddPushed(); break;
		case 5: OnRemovePushed(); break;
		case 6: OnCheckoutPushed(); break;
		}
		return 0;

	case WM_CLOSE:
		ShowWindow(m_hwnd, 0);
		return 0;

	case WM_DESTROY:
		DeleteObject(font);		//Same thing as deleting a pointer, just different because windows
		return 0;

	default:
		return DefWindowProcW(m_hwnd, msg, wParam, lParam);
	}
}