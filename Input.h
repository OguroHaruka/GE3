#pragma once
#include <windows.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <wrl.h>

//using namespace Microsoft::WRL;

class Input
{
public:
	void Initialize(HINSTANCE hInstance,HWND hend);

	void Update();

	bool PushKey(BYTE keyNumber);

	bool Trigger(BYTE keyNumber);

private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	// DirectInput�̏�����
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;

	BYTE key[256] = {};

	BYTE keyPre[256] = {};

};

