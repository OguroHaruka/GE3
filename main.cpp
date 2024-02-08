
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include"Sprite.h"

// ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    // メッセージ応じてゲーム固有の処理を行う
    switch (msg) {
        // ウィンドウが破棄された
    case WM_DESTROY:
        // OSに対して、アプリの終了を伝える
        PostQuitMessage(0);
        return 0;
    }

    // 標準のメッセージ処理を行う
    return DefWindowProc(hwnd, msg, wparam, lparam);
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    Input* input_ = nullptr;
    WinApp* winApp_ = nullptr;
    DirectXCommon* dxCommon_ = nullptr;

#pragma region WindowsAPI初期化処理
    
    winApp_ = new WinApp();
    winApp_->initialize();

#pragma endregion

#pragma region DirectX初期化処理
    

    dxCommon_ = new DirectXCommon;
    dxCommon_->Initialize(winApp_);
#pragma endregion

    input_ = new Input();
    input_->Initialize(winApp_);

    SpriteCommon* spriteCommon = new SpriteCommon;
    spriteCommon->Initialize(dxCommon_);

    Sprite* sprite = new Sprite();
    sprite->Initialize(dxCommon_,spriteCommon);

#pragma region 描画初期化処理

    

#pragma endregion

    
    // ゲームループ
    while (true) {

        if (winApp_->Update() == true) {
            break;
        }

        input_->Update();

        //更新前処理
        dxCommon_->PreDraw();

        sprite->Draw();

        //更新後処理
        dxCommon_->PostDraw();

        // DirectX毎フレーム処理　ここまで

    }

    delete sprite;
    delete spriteCommon;

    winApp_->Finalize();

    delete input_;
    delete winApp_;
    delete dxCommon_;

    return 0;
}
