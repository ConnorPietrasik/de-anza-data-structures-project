#pragma once
#include<Windows.h>
#include"GUI/BaseWindow.h"
#include"GUI/DisplayProductWindow.h"
#include"GUI/TextOutputWindow.h"

#include"Product.h"
#include"TArray.h"
#include"Catalog.h"

class ProductListWindow : public BaseWindow<ProductListWindow> {
public:
	enum MODE {
		VIEWMODE,
		DELMODE,
		CARTMODE
	};
private:
	Catalog* cat;
	DisplayProductWindow disp;
	TArray<Product*, CompareName, EqualityName, PrintName> curProds;
	MODE mode;
	TextOutputWindow textWin;

	HFONT font;

	void Init();
	void Paint();
	void Resize();

	void OnViewPushed();
	void OnDelPushed();
	void OnAddPushed();
	void OnRemovePushed();
	void OnCheckoutPushed();
public:
	PCWSTR ClassName() const { return L"Product List Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	void FillList(TArray<Product*, CompareName, EqualityName, PrintName> = TArray<Product*, CompareName, EqualityName, PrintName>());
	void SetMode(MODE = VIEWMODE);
	void SetCat(Catalog* x) { cat = x;}
};