#include<Windows.h>
#include"GUI/MainWindow.h"
#include"GUI/TextOutputWindow.h"
#include"GUI/FindProductWindow.h"
#include"GUI/ProductListWindow.h"

#include"Product.h"
#include"BST.h"
#include<string>
#include<sstream>
#include"FileManager.h"
#include"Catalog.h"

//Sets everything up
void MainWindow::Init() {
	//Checks to make sure that the file manager has data, and if it doesn't it asks where the file is
	bool good = fileMan.hasData();
	while (!good) {
		OPENFILENAMEA file = {};			//The 'A' at the end means "ANSI", most windows stuff uses wide but normal strings are ansi and I didn't feel like converting
		char buffer[260];
		file.lStructSize = sizeof(file);		//tbh I don't know why it can't figure out its own size, but it doesn't work without this
		file.lpstrFile = buffer;				//The place it saves the chosen file's path
		file.lpstrFile[0] = '\0';				//Have to set the beginning of the buffer to null so it knows not to try to use it
		file.nMaxFile = sizeof(buffer);			//Max file path length, just in case
		file.lpstrFilter = "*.tsv\0";			//This makes the thing in the bottom right say ".tsv" so you know what file type to find
		file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;	//You can't select a file that doesn't exist

		if (GetOpenFileNameA(&file)) {
			std::string path(file.lpstrFile);
			if (fileMan.read(path)) good = true;
			else MessageBoxW(m_hwnd, L"The file was not able to be read, please check the file and try again", L"ERROR", MB_OK | MB_ICONERROR);
		}
		else {
			if (MessageBoxW(m_hwnd, L"No products will be loaded, you must add them yourself.\r\nDo you wish to proceed without loading a file?", L"Are you sure?", MB_YESNO | MB_ICONQUESTION) == IDYES) good = true;
		}
	}
	cat.setProductList(fileMan.getProductList());
	cat.buildTree();
	cat.buildHash();
	cat.setCart(new ShoppingCart);

	//Set the default size of the main window
	SetWindowPos(
		m_hwnd,		//A handle to the current window, it was set in Basewindow's thing
		HWND_TOP,	//A flag that says it's the top window
		GetSystemMetrics(SM_CXSCREEN) / 7,			//X pos
		GetSystemMetrics(SM_CYSCREEN) / 5,			//Y pos
		400,		//Width
		600,		//Height
		NULL		//No extra flags (which would say things like "don't move me, don't resize me," etc
	);

	//The button labels in order
	const wchar_t* btnLbls[] = {L"Add New Product", L"Delete Product", L"Find Product", L"List Products (HashTable)", L"List Products (Sorted)", L"Show Tree", L"Efficiency", L"View Cart", L"Quit"};
	
	//Creates the buttons
	for (int i = 0; i < 9; i++) {
		CreateWindowW(
			L"BUTTON",
			btnLbls[i],		//The button text
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,	//Flags that basically just say it's a normal button
			0, 0, 0, 0,		//The position and size are set later
			m_hwnd,
			(HMENU)(i + 1),	//The button's ID, for example the button labeled "Add New Product" will have ID 1
			GetModuleHandleW(NULL),
			NULL			//This is where you could pass a pointer to give it more data, like in BaseWindow's create
		);
	}

	//The GetSystemMetrics just means that it goes near the middle of the screen, the flags at the end are window styles and then window extended styles
	textWin.Create(L"Output", 0, 0, GetSystemMetrics(SM_CXSCREEN) / 3, GetSystemMetrics(SM_CYSCREEN) / 4, m_hwnd, WS_OVERLAPPEDWINDOW | WS_POPUP, WS_EX_DLGMODALFRAME);
	findWin.Create(L"Find Product", 250, 230, GetSystemMetrics(SM_CXSCREEN) / 3, GetSystemMetrics(SM_CYSCREEN) / 4, m_hwnd, WS_CAPTION | WS_SYSMENU | WS_POPUP, WS_EX_DLGMODALFRAME);
	listWin.Create(L"List of Products", 400, 500, GetSystemMetrics(SM_CXSCREEN) / 3, GetSystemMetrics(SM_CYSCREEN) / 4, m_hwnd, WS_OVERLAPPEDWINDOW | WS_POPUP, WS_EX_DLGMODALFRAME);
	addWin.Create(L"Add Product (* indicates required field)", 700, 280, GetSystemMetrics(SM_CXSCREEN) / 3, GetSystemMetrics(SM_CYSCREEN) / 4, m_hwnd, WS_OVERLAPPEDWINDOW | WS_POPUP, WS_EX_DLGMODALFRAME);

	//Gives the windows a pointer to the catalog
	findWin.SetCat(&cat);
	listWin.SetCat(&cat);
	addWin.SetCat(&cat);
}

//Fills the background of the window with basic background color
void MainWindow::Paint() {
	PAINTSTRUCT ps = {};
	HDC hdc = BeginPaint(m_hwnd, &ps);	//Fills ps with info about repaint request, ps.rcPaint = update region
	FillRect(hdc, &ps.rcPaint, (HBRUSH)COLOR_BACKGROUND);	//Fills the update region with grey
	EndPaint(m_hwnd, &ps);	//Clears the update region, telling the OS it's done
}

//Sets everything to the proper size / position
void MainWindow::Resize() {
	RECT size;
	GetClientRect(m_hwnd, &size);	//Sets size to the client area (the area of the window not counting the border or the bar at the top)

	long height = (size.bottom - (2 * (size.bottom / 40))) / 6;
	long leftX = (size.right / 40);
	long rightX = (size.right / 2);
	long gap = (size.bottom / 40);
	long leftWidth = (size.right / 2) - (size.right / 20);
	long rightWidth = (size.right / 2) - (size.right / 40);

	//Sets the button positions
	for (int i = 1, j = 1; i < 9; i++) {
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

	//Sets Quit position, separate because not in the columns
	SetWindowPos(
		GetDlgItem(m_hwnd, 9),
		NULL,
		size.right / 20,
		(5 * gap) + (4 * height),
		size.right - (2 * (size.right / 20)),
		height + (2 * gap),
		SWP_NOZORDER
	);
}

//Sets the text output window's text to the BST's print
void MainWindow::OnTreeButton() {
	std::wstring title = GetWindowTitle(textWin.Window());

	//Checks if it's currently showing the tree, and hides it if it is
	if (IsWindowVisible(textWin.Window()) && title == L"Tree") {
		ShowWindow(textWin.Window(), 0);
	}
	else {
		cat.getBST()->setPrint();
		std::wstring tree = ConvertToWString(cat.getBST()->getPrint());
		
		SetWindowTextW(GetDlgItem(textWin.Window(), 1), tree.c_str());
		SetWindowTextW(textWin.Window(), L"Tree");
		ShowWindow(textWin.Window(), 1);
	}
}

//Sets the find window to delete mode
void MainWindow::OnDeleteButton() {
	std::wstring title = GetWindowTitle(findWin.Window());

	if (IsWindowVisible(findWin.Window()) && title == L"Delete Product") {
		ShowWindow(findWin.Window(), 0);
	}
	else {
		findWin.SetDelMode(true);
		ShowWindow(findWin.Window(), 1);
	}
}

//Sets the find window to view mode
void MainWindow::OnFindButton() {
	std::wstring title = GetWindowTitle(findWin.Window());

	if (IsWindowVisible(findWin.Window()) && title == L"Find Product") {
		ShowWindow(findWin.Window(), 0);
	}
	else {
		findWin.SetDelMode(false);
		ShowWindow(findWin.Window(), 1);
	}
}

//Shows the products in BST order
void MainWindow::OnSortedListButton() {
	std::wstring title = GetWindowTitle(listWin.Window());

	if (IsWindowVisible(listWin.Window()) && title == L"Sorted Products") {
		ShowWindow(listWin.Window(), 0);
	}
	else {
		listWin.SetMode(ProductListWindow::VIEWMODE);
		listWin.FillList(cat.getBST()->getAllData());
		SetWindowTextW(listWin.Window(), L"Sorted Products");
		ShowWindow(listWin.Window(), 1);
	}
}

//Shows the add window
void MainWindow::OnAddButton() {
	if (IsWindowVisible(addWin.Window())) {
		ShowWindow(addWin.Window(), 0);
	}
	else {
		ShowWindow(addWin.Window(), 1);
	}
}

//Shows the products in hash table order
void MainWindow::OnHashListButton() {
	std::wstring title = GetWindowTitle(listWin.Window());

	if (IsWindowVisible(listWin.Window()) && title == L"Products Hashed Order") {
		ShowWindow(listWin.Window(), 0);
	}
	else {
		TArray<Product*, CompareName, EqualityName, PrintName> arr;
		
		//Have to convert because getAllProducts's TArray is using default functors
		auto temp = cat.getHash()->getAllProducts();
		for (auto p : temp) arr.push_back(p);

		listWin.SetMode(ProductListWindow::VIEWMODE);
		listWin.FillList(arr);
		SetWindowTextW(listWin.Window(), L"Products Hashed Order");
		ShowWindow(listWin.Window(), 1);
	}
}

//Shows the efficiency of the hash table and the hash table's data
void MainWindow::OnEfficiencyButton() {
	std::wstring title = GetWindowTitle(textWin.Window());

	if (IsWindowVisible(textWin.Window()) && title == L"Efficiency") {
		ShowWindow(textWin.Window(), 0);
	}
	else {
		std::string temp;
		
		//The stringstreams have their own scope because ignore was being weird, and time's running out
		{
			std::stringstream ss;
			cat.getHash()->displayEfficiency(ss);
			temp = ss.str();
		}
		std::wstring efficiency = ConvertToWString(temp);

		{
			std::stringstream ss;
			ss << *(cat.getHash());
			temp = "\n\n" + ss.str();
		}
		std::wstring hashData = ConvertToWString(temp);

		SetWindowTextW(GetDlgItem(textWin.Window(), 1), (efficiency + hashData).c_str());
		SetWindowTextW(textWin.Window(), L"Efficiency");
		ShowWindow(textWin.Window(), 1);
	}
}

//Shows the cart
void MainWindow::OnCartButton() {
	std::wstring title = GetWindowTitle(listWin.Window());

	if (IsWindowVisible(listWin.Window()) && title == L"Products in Cart") {
		ShowWindow(listWin.Window(), 0);
	}
	else {
		TArray<Product*, CompareName, EqualityName, PrintName> arr;
		for (auto p : cat.getCart()->getCart()) arr.push_back(p);

		listWin.SetMode(ProductListWindow::CARTMODE);
		listWin.FillList(arr);
		SetWindowTextW(listWin.Window(), L"Products in Cart");
		ShowWindow(listWin.Window(), 1);
	}
}

//Saves to the file that was input, or if no input file was given it asks where you want to save (basically same as the open file thing)
void MainWindow::OnQuitButton() {
	std::string path;
	bool good = fileMan.save();
	while (!good) {
		OPENFILENAMEA file = {};
		char buffer[260];
		file.lStructSize = sizeof(file);
		file.lpstrFile = buffer;
		file.lpstrFile[0] = '\0';
		file.nMaxFile = sizeof(buffer);
		file.lpstrFilter = "*.tsv\0";
		if (GetSaveFileNameA(&file)) {
			std::string path(file.lpstrFile);
			if (fileMan.save(path)) good = true;
			else MessageBoxW(m_hwnd, L"The file was not able to be saved, please select a different location.", L"ERROR", MB_OK | MB_ICONERROR);
		}
		else {
			if (MessageBoxW(m_hwnd, L"No products will be saved, is that okay?", L"Exit without saving?", MB_YESNO | MB_ICONQUESTION) == IDYES) good = true;
		}
	}
	DestroyWindow(m_hwnd);
}

//The message handler for the main window
LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_CREATE:		//Called when window is created
		Init();
		return 0;

	case WM_PAINT: 	//Called when window is updated
		Paint();
		return 0;

	case WM_CLOSE: 	//Called when the red X is clicked, alt+f4, etc
		MessageBoxW(m_hwnd, L"Click the quit button to quit\r\nI made it for a reason", L"No.", MB_OK | MB_ICONERROR);	//I could just have this call OnQuitButton, but quit is a required menu item so I demand the user uses it
		return 0;

	case WM_DESTROY: 	//Called when window is destroyed
	{
		PostQuitMessage(0);		//Tells the message loop to stop
		return 0;
	}

	case WM_SIZE:		//Called when window is resized
		Resize();
		return 0;

	case WM_COMMAND:	//Called when a button is pushed (or other control stuff happens, but this one only has buttons)
		switch (LOWORD(wParam)) {	//The button's ID
		case 1: OnAddButton(); break;
		case 2: OnDeleteButton(); break;
		case 3: OnFindButton(); break;
		case 4: OnHashListButton(); break;
		case 5: OnSortedListButton(); break;
		case 6: OnTreeButton(); break;
		case 7: OnEfficiencyButton(); break;
		case 8: OnCartButton(); break;
		case 9: OnQuitButton(); break;
		}
		return 0;

	default:
		return DefWindowProcW(m_hwnd, msg, wParam, lParam);	//Does whatever the default is for the message
	}
}