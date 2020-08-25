#pragma once
#include<Windows.h>
#include"GUI/BaseWindow.h"

class DeleteProductWindow : public BaseWindow<DeleteProductWindow> {
	void Init();
	void Paint();
	void OnDelete();
public:
	PCWSTR ClassName() const { return L"Delete Product Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};