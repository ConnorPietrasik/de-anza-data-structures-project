#pragma once
#include<Windows.h>
#include"GUI/BaseWindow.h"
#include"Product.h"

class DisplayProductWindow : public BaseWindow<DisplayProductWindow> {
	void Init();
	void Paint();
	void Resize();

public:
	PCWSTR ClassName() const { return L"Display Product Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	void setProduct(Product*);
};