#pragma once
#include<Windows.h>
#include"GUI/BaseWindow.h"
#include"BST.h"
#include"Product.h"
#include"GUI/ProductListWindow.h"
#include"Catalog.h"

class FindProductWindow : public BaseWindow<FindProductWindow>{
	void Init();
	void Paint();
	void OnName();
	void OnUPC();
	Catalog* cat;

	bool delMode;

	ProductListWindow disp;
public:
	void SetCat(Catalog* x) { cat = x; disp.SetCat(x); }
	void SetDelMode(bool x = true);

	PCWSTR ClassName() const { return L"Find Product Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};