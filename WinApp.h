#pragma once
#include <Windows.h>

class WinApp
{
public:

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void initialize();

	void Update();

};

