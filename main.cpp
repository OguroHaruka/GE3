
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include <vector>

#include"TextureManager.h"

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

    ImGuiManager* imgui = ImGuiManager::Create();
    ImGuiManager::Initialize(winApp_->GetHwnd(), dxCommon_);

    SpriteCommon* spriteCommon = new SpriteCommon;
    spriteCommon->Initialize(dxCommon_);

    TextureManager::GetInstance()->Initialize(dxCommon_);
    TextureManager::GetInstance()->LoadTexture(L"resources/mario.jpg");
    TextureManager::GetInstance()->LoadTexture(L"resources/reimu.png");

    std::vector<Sprite*>sprite;
    for (int i = 0; i < 5; i++) {
        Sprite* temp = new Sprite();
        if(i%2==0)
        temp->Initialize(spriteCommon, L"resources/mario.jpg");
        else if(i%2==1)
            temp->Initialize(spriteCommon, L"resources/reimu.png");
        temp->SetPosition({ (float)i * 120,0 });
        sprite.push_back(temp);
    }
#pragma region 描画初期化処理

    

#pragma endregion

    
    // ゲームループ
    while (true) {

        if (winApp_->Update() == true) {
            break;
        }
        ImGuiManager::NewFrame();
        imgui->ShowDemo();

        input_->Update();

        //DirectX::XMFLOAT2 pos = sprite->GetPosition();
        //pos.x += 0.01f;
        //sprite->SetPosition(pos);

        //float rot = sprite->GetRotation();
        //rot += 0.001f;
        //sprite->SetRotation(rot);

        //DirectX::XMFLOAT4 color = sprite->GetColor();
        //color.x -= 0.01f;
        //if (color.x < 0) {
        //    color.x = 1.0f;
        //}
        //sprite->SetColor(color);

        //DirectX::XMFLOAT2 size = sprite->GetSize();
        //size.y += 0.01f;
        //sprite->SetSize(size);

        for (int i = 0; i < 5; i++) {
            sprite[i]->Update();
        }
        //更新前処理
        ImGuiManager::CreateCommand();
        dxCommon_->PreDraw();
        spriteCommon->SpritePreDraw();

        for (int i = 0; i < 5; i++) {
            sprite[i]->Draw();
        }
        //更新後処理
        ImGuiManager::CommandsExcute(dxCommon_->GetCommandList());
        dxCommon_->PostDraw();

        // DirectX毎フレーム処理　ここまで

    }
    for (int i = 0; i < 5; i++) {
        delete sprite[i];
    }
    TextureManager::GetInstance()->Finalize();
    delete spriteCommon;

    delete imgui;

    winApp_->Finalize();

    delete input_;
    delete winApp_;
    delete dxCommon_;

    return 0;
}
