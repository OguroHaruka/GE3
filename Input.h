#pragma once
#include <windows.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <wrl.h>
#include "WinApp.h"

//using namespace Microsoft::WRL;

class Input
{
public:
	void Initialize(WinApp*winApp);

	void Update();

	bool PushKey(BYTE keyNumber);

	bool Trigger(BYTE keyNumber);

private:
	WinApp* winApp_ = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	// DirectInputの初期化
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;

	BYTE key[256] = {};

	BYTE keyPre[256] = {};

};

