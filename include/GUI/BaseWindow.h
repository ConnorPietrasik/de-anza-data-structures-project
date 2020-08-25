#pragma once

#include<Windows.h>
#include<string>
#include<sstream>



template<typename DerivedWindow>
class BaseWindow {
protected:
	virtual PCWSTR ClassName() const = 0;	//The name of the window class
	virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;	//Message handler that will be called
	HWND m_hwnd;	//Handle to the window (can basically think of handles as void*, not exactly correct but good enough)

public:
	BaseWindow() : m_hwnd(nullptr) {}
	std::wstring GetWindowTitle(HWND hwnd);
	std::wstring ConvertToWString(const std::string&);

	static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
	int Create(PCWSTR lpWindowName, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, HWND hwndParent = 0, DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0, HMENU hMenu = 0);
	HWND Window() { return m_hwnd; }	//Returns the handle to the window
};

//Basically just lets the derived window handle the message, with some setup at the beginning
template<typename DerivedWindow>
LRESULT CALLBACK BaseWindow<DerivedWindow>::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	DerivedWindow* pThis = nullptr;

	if (msg == WM_CREATE) {	//If the window is first being created
		CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);	//Turns the void * into a CREATESTRUCT pointer
		pThis = reinterpret_cast<DerivedWindow*>(pCreate->lpCreateParams);	//Sets pThis to a pointer to the derived window (in Create, 'this' is passed)
		SetWindowLongW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));	//Puts pThis in the GWLP_USERDATA spot of hwnd (lets it be obtained using GetWindowLong)

		pThis->m_hwnd = hwnd;	//Sets the m_hwnd handle in the derived window to a handle to itself
	}
	else pThis = reinterpret_cast<DerivedWindow*>(GetWindowLongW(hwnd, GWLP_USERDATA));	//Sets pThis to a pointer to the derived window

	if (pThis) return pThis->HandleMessage(msg, wParam, lParam);	//Lets the derived class handle the message
	else return DefWindowProcW(hwnd, msg, wParam, lParam);	//Do the default if pThis failed to be a pointer
}

//does the actual window creation
template<typename DerivedWindow>
int BaseWindow<DerivedWindow>::Create(		//Creates the window with the given input
	PCWSTR lpWindowName,	//The name of the window, it's the text in the title bar
	int nWidth,		//Width in device units
	int nHeight,	//Height in device units
	int x,	//X position from top left
	int y,	//Y position from top left
	HWND hwndParent,	//Handle to parent window
	DWORD dwStyle,	//The window style
	DWORD dwExStyle,	//Extra window style stuff
	HMENU hMenu		//Handle to menu or child window
){
	WNDCLASSW wc = {};
	wc.lpfnWndProc = DerivedWindow::WindowProc;		//Sets the Window procedure to the derived window's
	wc.hInstance = GetModuleHandleW(NULL);	//Sets the hInstance to that of the EXE
	wc.lpszClassName = ClassName();

	RegisterClassW(&wc);	//Registers the class with the OS

	m_hwnd = CreateWindowExW(
		dwExStyle,
		ClassName(),
		lpWindowName,
		dwStyle,
		x,
		y,
		nWidth,
		nHeight,
		hwndParent,
		hMenu,
		GetModuleHandleW(NULL),
		this
	);	//Creates the window using the given params

	return (m_hwnd ? true : false);	//Returns false if it fails for whatever reason
}

//Gets the window title (or text if it's a textbox (edit control))
template<typename DerivedWindow>
std::wstring BaseWindow<DerivedWindow>::GetWindowTitle(HWND hwnd) {
	int len = GetWindowTextLengthW(hwnd) + 1;
	wchar_t* buffer = new wchar_t[len];
	GetWindowTextW(hwnd, buffer, len);
	std::wstring title(buffer);
	delete[] buffer;
	return title;
}

//Converts a string to a wString, and adds '\r' in front of '\n' because Windows is weird and needs it
template<typename DerivedWindow>
std::wstring BaseWindow<DerivedWindow>::ConvertToWString(const std::string& str) {
	std::stringstream ss;
	for (auto c : str) {
		if (c == '\n') ss << '\r';
		ss << c;
	}
	std::string temp = ss.str();
	std::wstring ret(temp.begin(), temp.end());
	return ret;
}