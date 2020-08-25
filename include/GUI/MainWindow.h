#pragma once
#include<Windows.h>
#include"GUI/BaseWindow.h"
#include"GUI/TextOutputWindow.h"
#include"GUI/FindProductWindow.h"
#include"GUI/ProductListWindow.h"
#include"GUI/AddProductWindow.h"

#include"FileManager.h"
#include"Catalog.h"

class MainWindow : public BaseWindow<MainWindow>{
	TextOutputWindow textWin;
	FindProductWindow findWin;
	ProductListWindow listWin;
	AddProductWindow addWin;


	FileManager fileMan;
	Catalog cat;

	void Init();
	void Paint();
	void Resize();

	void OnTreeButton();
	void OnDeleteButton();
	void OnFindButton();
	void OnSortedListButton();
	void OnAddButton();
	void OnHashListButton();
	void OnEfficiencyButton();
	void OnCartButton();
	void OnQuitButton();
public:
	PCWSTR ClassName() const { return L"Main Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};