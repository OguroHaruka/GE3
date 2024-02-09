#include "Sprite.h"
#include "BufferResource.h"
#include "External/imgui/imgui.h"

#include <DirectXMath.h>
using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize( SpriteCommon* common)
{
	common_ = common;
	dxCommon_ = common_->GetDirectXCommon();

	//‰æ‘œ‚Ì“Ç‚Ýž‚Ý
	DirectX::ScratchImage mipImages = common->LoadTexture(L"Resources/mario.jpg");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(dxCommon_->GetDevice(), metaData);
	common_->UploadTextureData(textureResource, mipImages);

	//SHaderResourceViewì¬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metaData.mipLevels);

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU = dxCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	
	textureSrvHandleCPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);

	CreateVertex();

	CreateMaterial();

	CreateWVP();
}

void Sprite::Draw()
{

	//transform_.rotate.y += 0.03f;

	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform_.scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform_.rotate));
	XMMATRIX tanslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform_.translate));

	XMMATRIX rotationAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);
	XMMATRIX worldMatrix = XMMatrixMultiply(rotationAndScaleMatrix, tanslationMatrix);

	XMMATRIX cameraScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&cameraTransform.scale));
	XMMATRIX cameraRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraTransform.rotate));
	XMMATRIX cameraTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&cameraTransform.translate));

	XMMATRIX cameraRotateAndScaleMatrix = XMMatrixMultiply(cameraRotateMatrix,cameraScaleMatrix);
	XMMATRIX cameraMatrix = XMMatrixMultiply(cameraRotateAndScaleMatrix, cameraTranslationMatrix);

	XMMATRIX view = XMMatrixInverse(nullptr, cameraMatrix);
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)WinApp::window_width / (float)WinApp::window_height,0.1f,100.0f);

	XMMATRIX worldViewProjectionMatrix = worldMatrix * (view * proj);

	*wvpData = worldViewProjectionMatrix;

	
	//’¸“_î•ñ
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexbufferView);

	
	//Fî•ñ
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//s—ñ
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	//‰æ‘œ
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);

	dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);

}

void Sprite::CreateVertex()
{
	vertexResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 6);

	vertexbufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexbufferView.SizeInBytes = sizeof(VertexData) * 6;
	vertexbufferView.StrideInBytes = sizeof(VertexData);

	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[1].position = {-0.5f,+0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[2].position = { +0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };

	vertexData[3].position = { -0.5f,+0.5f,0.0f,1.0f };
	vertexData[3].texcoord = { 0.0f,0.0f };
	vertexData[4].position = { +0.5f,+0.5f,0.0f,1.0f };
	vertexData[4].texcoord = { 1.0f,0.0f };
	vertexData[5].position = { +0.5f,-0.5f,0.0f,1.0f };
	vertexData[5].texcoord = { 1.0f,1.0f };


}

void Sprite::CreateMaterial()
{
	materialResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMFLOAT4));

	XMFLOAT4* materialData = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	*materialData =color_;
}

void Sprite::CreateWVP()
{
	wvpResource = CreateBufferResource(dxCommon_->GetDevice(), sizeof(XMMATRIX));
	
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	*wvpData = XMMatrixIdentity();
}
