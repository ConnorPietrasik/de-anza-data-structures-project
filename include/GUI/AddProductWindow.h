#pragma once
#include<Windows.h>
#include"GUI/BaseWindow.h"
#include"Catalog.h"
#include"Product.h"
#include"GUI/DisplayProductWindow.h"

class AddProductWindow : public BaseWindow<AddProductWindow> {
	Catalog* cat;
	DisplayProductWindow disp;

	void Init();
	void Paint();
	void Resize();

	void OnAddButton();
public:
	PCWSTR ClassName() const { return L"Add Product Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	void SetCat(Catalog* x) { cat = x; }
};