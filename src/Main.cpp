#define WINVER 0x0601		//Makes sure it works with Windows 7

#include<Windows.h>
#include"GUI/MainWindow.h"

// ORDER OF READING FOR UNDERSTANDING GUI IS BASEWINDOW.H, THEN MAINWINDOW.CPP, THEN THE .CPP FILES IN ALPHABETICAL ORDER
// (or do whatever you want, but I tried to avoid explaining things multiple times)

//Have to use wWinMain instead of main because it's a Windows application
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInsance, PWSTR pCmdLine, int nCmdShow) {

	MainWindow winMain;		//Each window is its own class inherited from BaseWindow
	if (!winMain.Create(L"Main Window")) return -1;		//If the main window can't be created for whatever reason, just end the process with an error code

	ShowWindow(winMain.Window(), nCmdShow);		//Displays the window

	MSG msg = {};								//MSG object to be filled
	while (GetMessageW(&msg, NULL, 0, 0) > 0) {	//GetMessage &msg fills the message, other params are filters, returns 0 on quit and -1 on error
		TranslateMessage(&msg);					//Translates keystrokes into chars
		DispatchMessageW(&msg);					//Tells the OS to call the WindowsProc
	}

	return (int)msg.wParam;		//Returns an error if something broke, 0 if normal
}