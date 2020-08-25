#pragma once
#include<Windows.h>
#include"GUI/BaseWindow.h"

class TextOutputWindow : public BaseWindow<TextOutputWindow>{
	void Init();
	void Resize();
public:
	PCWSTR ClassName() const { return L"TextOutputWindow"; }
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};